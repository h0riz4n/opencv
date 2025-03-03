#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;


int main() {
  Mat image = imread("./image.jpg");

  if (image.empty()) {
    cout << "Не удалось загрузить изображение!" << endl;
    return -1;
  }

	// ЧАСТЬ 1: Разные цветовые пространства

	// Mat hsvImage, labImage, yuvImage, xyzImage, grayImage, gausImage;

	// cvtColor(image, hsvImage, COLOR_BGR2HSV);
	// cvtColor(image, labImage, COLOR_BGR2Lab);
	// cvtColor(image, yuvImage, COLOR_BGR2YUV);
	// cvtColor(image, xyzImage, COLOR_BGR2XYZ);
	// cvtColor(image, grayImage, COLOR_BGR2GRAY);

	// GaussianBlur(grayImage, gausImage, Size(9, 9), 2, 2);

	// Вывод изображений в разных цветовых пространствах
	// cv::imshow("Original Image", image);
	// cv::imshow("HSV Space", hsvImage);
	// cv::imshow("LAB Space", labImage);
	// cv::imshow("YUV Space", yuvImage);
	// cv::imshow("XYZ Space", xyzImage);
	// cv::imshow("Gray Space", grayImage);
	// cv::imshow("Gaussian Filter", gausImage);


	// ЧАСТЬ 2: Определение кругов и линий

	// Перевод изображения в фильтр Гаусса для удобства
	// Mat grayImage, gausImage;

	// cvtColor(image, grayImage, COLOR_BGR2GRAY);
	// GaussianBlur(grayImage, gausImage, Size(9, 9), 2, 2);

	// Определение линий
	// Mat edges;
	// Canny(gausImage, edges, 50, 150, 3);

	// vector<Vec2f> lines;
	// HoughLines(edges, lines, 1, CV_PI / 180, 80);

	// for (size_t i = 0; i < lines.size(); i++) {
	// 	float rho = lines[i][0];
	// 	float theta = lines[i][1];

	// 	double a = cos(theta), b = sin(theta);
	// 	double x0 = a * rho, y0 = b * rho;

	// 	Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
	// 	Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
	// 	line(image, pt1, pt2, Scalar(0, 0, 255), 2, LINE_AA);
	// }

	// Определение окружностей
	// vector<Vec3f> circles;
	// HoughCircles(gausImage, circles, HOUGH_GRADIENT, 1.2, 30, 100, 30, 10, 100);

	// for (size_t i = 0; i < circles.size(); i++) {
	// 	Vec3i c = circles[i];
	// 	Point center(c[0], c[1]);
	// 	int radius = c[2];

	// 	circle(image, center, 3, Scalar(0, 255, 0), -1, LINE_AA);
	// 	circle(image, center, radius, Scalar(255, 0, 0), 2, LINE_AA);
	// }

	// Вывод изображения
	// imshow("Circles And Lines", image);

	// ---Определение красного диапазона цветов на картинке---

	// Перевод изображения в HSV пространство для удобства
	Mat hsvImage;
	cvtColor(image, hsvImage, COLOR_BGR2HSV);

	Mat mask, mask1, mask2, result;

	// Определение красного цвета на картинке и создание маски
	inRange(hsvImage, Scalar(0, 100, 100), Scalar(10, 255, 255), mask1);
	inRange(hsvImage, Scalar(160, 100, 100), Scalar(180, 255, 255), mask2);

	// Наложение маски на оригинальное изображение
	bitwise_or(mask1, mask2, mask);
	bitwise_and(image, image, result, mask);

	imshow("Original Image", image);
	imshow("Mask", mask);
	imshow("Result Image", result);


  	waitKey(0);
	destroyAllWindows();

  return 0;
}
