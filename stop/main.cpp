#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main() {
  Mat image = imread("./stop.png");

  if (image.empty()) {
    cout << "Не удалось загрузить изображение!" << endl;
    return -1;
  }

	// Ресайз картинки для удобства
	resize(image, image, Size(1000, 700), INTER_LINEAR);


	// Перевод в другие цветовые пространства для удобства
	Mat hsvImage, grayImage;
	cvtColor(image, hsvImage, COLOR_BGR2HSV);
	cvtColor(image, grayImage, COLOR_BGR2GRAY);
	GaussianBlur(grayImage, grayImage, Size(9, 9), 2);

	// Определение красного цвета на картинке и создание маски
	Mat mask, mask1, mask2, result;
	inRange(hsvImage, Scalar(0, 100, 100), Scalar(10, 255, 255), mask1);
	inRange(hsvImage, Scalar(160, 100, 100), Scalar(180, 255, 255), mask2);

	// Наложение маски на оригинальное изображение
	bitwise_or(mask1, mask2, mask);
	bitwise_and(image, image, result, mask);

	// Определение окружностей
	vector<Vec3f> circles;
	HoughCircles(grayImage, circles, HOUGH_GRADIENT, 1, grayImage.rows, 55, 45, 200, 280);

	for (size_t i = 0; i < circles.size(); i++) {
		Vec3i c = circles[i];
		Point center(c[0], c[1]);
		int radius = c[2];

		circle(result, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
		circle(result, center, radius, Scalar(255, 0, 0), 2, LINE_AA);
	}

	imshow("Original Image", image);
	imshow("Mask", mask);
	imshow("Result Image", result);

  waitKey(0);
	destroyAllWindows();

  return 0;
}
