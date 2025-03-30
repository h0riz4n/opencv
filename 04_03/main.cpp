#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Определение четырехугольных фигур
string detectQuadrilateral(const vector<Point>& approx) {
	Rect rect = boundingRect(approx);
	float aspectRatio = static_cast<float>(rect.width) / rect.height;
	return (aspectRatio >= 0.9 && aspectRatio <= 1.1) ? "Square" : "Rectangle";
}

// Определение многоугольников
string detectPolygon(const vector<Point>& approx) {
	return (approx.size() < 7) ? "Polygon" : "Circle";
}

// Определение фигуры
string detectShape(const vector<Point>& contour) {
	vector<Point> approx;
	approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
	
	switch (approx.size()) {
		case 3: return "Triangle";
		case 4: return detectQuadrilateral(approx);
		default: return detectPolygon(approx);
	}
}

int main() {
  Mat image = imread("./image1.png");

  if (image.empty()) {
    cout << "Не удалось загрузить изображение!" << endl;
    return -1;
  }

	Mat hsvImage, grayImage;
	cvtColor(image, hsvImage, COLOR_BGR2HSV);
	cvtColor(hsvImage, grayImage, COLOR_BGR2GRAY);

	Mat edges;
	Canny(grayImage, edges, 50, 150);

	vector<vector<Point>> contours;
	findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (const auto& contour : contours) {
		string shape = detectShape(contour);
		drawContours(image, vector<vector<Point>>{contour}, -1, Scalar(0, 255, 0), 2);
		Moments m = moments(contour);

		if (m.m00 != 0) {
			int cx = static_cast<int>(m.m10 / m.m00);
			int cy = static_cast<int>(m.m01 / m.m00);
	
			putText(image, shape, Point(cx, cy), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 0), 2);
		}
	}

	imshow("Shape Detection", image);
  waitKey(0);
	
  return 0;
}
