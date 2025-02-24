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

	int height = image.rows;
	int width = image.cols;
	
	int halfHeight = height / 2;
	int halfWidth = width / 2;

	// Области изображения
	Mat topLeft = image(Rect(0, 0, halfWidth, halfHeight));
	Mat topRight = image(Rect(halfWidth, 0, halfWidth, halfHeight));
	Mat bottomLeft = image(Rect(0, halfHeight, halfWidth, halfHeight));
	Mat bottomRight = image(Rect(halfWidth, halfHeight, halfWidth, halfHeight));
	
	// Верхний левый - инверсия
	bitwise_not(topLeft, topLeft);
	
	// Нижний левый - размытие
	GaussianBlur(bottomLeft, bottomLeft, Size(51, 51), 0);
	
	// Нижний правый - контрасность
	bottomRight.convertTo(bottomRight, -1, 2, 0);
	
	cv::imshow("Effects", image);
	waitKey(0);
	
	return 0;
}