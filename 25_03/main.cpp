#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/calib3d.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    Mat image1 = imread("C:/Programming/C++/Image-Recognition/11.03/src/image1.png");
    Mat image2 = imread("C:/Programming/C++/Image-Recognition/11.03/src/image2.png");

    if (image1.empty() || image2.empty()) {
        cerr << "Не удалось загрузить изображения!" << endl;
        return -1;
    }

    Mat grayImage1, grayImage2;
    cvtColor(image1, grayImage1, COLOR_BGR2GRAY);
    cvtColor(image2, grayImage2, COLOR_BGR2GRAY);

    Ptr<ORB> detector = ORB::create();
    vector<KeyPoint> kpImage1, kpImage2;
    Mat descriptors1, descriptors2;

    detector->detectAndCompute(grayImage1, Mat(), kpImage1, descriptors1);
    detector->detectAndCompute(grayImage2, Mat(), kpImage2, descriptors2);

    BFMatcher matcher(NORM_HAMMING, true);
    vector<DMatch> rawMatches;
    matcher.match(descriptors1, descriptors2, rawMatches);

    double maxDist = 0.0, minDist = 100.0;

    // Поиск максимального и минимального расстояния между совпадениями
    for (const auto& match : rawMatches) {
        double dist = match.distance;
        maxDist = max(maxDist, dist);
        minDist = min(minDist, dist);
    }

    // Фильтрация хороших совпадений
    vector<DMatch> validMatches;
    for (const auto& match : rawMatches) {
        if (match.distance <= max(2 * minDist, 30.0)) {
            validMatches.push_back(match);
        }
    }

    // Извлечение координат точек для нахождения гомографии
    vector<Point2f> points1, points2;
    for (const auto& match : validMatches) {
        points1.push_back(kpImage1[match.queryIdx].pt);
        points2.push_back(kpImage2[match.trainIdx].pt);
    }

    // Вычисление матрицы гомографии
    Mat homography = findHomography(points2, points1, RANSAC);

    // Создание панорамы
    Mat panorama;
    warpPerspective(image2, panorama, homography, Size(image1.cols + image2.cols, image1.rows));
    Mat leftSide(panorama, Rect(0, 0, image1.cols, image1.rows));
    image1.copyTo(leftSide);

    // Отображение совпадений
    Mat matchImage;
    drawMatches(image1, kpImage1, image2, kpImage2, rawMatches, matchImage,
                    Scalar::all(-1), Scalar::all(-1),
                    std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // Показ результатов
    imshow("Совпадения", matchImage);
    imshow("Панорама", panorama);
    waitKey(0);

    return 0;
}
