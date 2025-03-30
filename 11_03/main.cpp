#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	VideoCapture video("./video.mp4");

	if (!video.isOpened()) {
		cout << "Ошибка при открытии видеофайла" << endl;
		return -1;
	}

	Mat frame, grayImage, blurredImage, edgesImage;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	while (true) {
		video >> frame;
		if (frame.empty()) break;

		cvtColor(frame, grayImage, COLOR_BGR2GRAY);	
		GaussianBlur(grayImage, blurredImage, Size(5, 5), 0);
		Canny(blurredImage, edgesImage, 50, 150, 3);

		findContours(edgesImage, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		Mat resultImage = frame.clone();

		for (const auto& contour : contours) {
			double perimeter = cv::arcLength(contour, true);

			std::vector<cv::Point> approx;
			approxPolyDP(contour, approx, 0.02 * perimeter, true);

			if (perimeter < 900.0 && cv::isContourConvex(approx) && approx.size() == 4) {
				drawContours(resultImage, std::vector<std::vector<cv::Point>>{approx}, -1, cv::Scalar(0, 0, 255), 2);
				putText(resultImage, "square", approx[0], cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
			}
		}

		imshow("Tracking", resultImage);

		if (waitKey(30) >= 0) break;
	}

	namedWindow("Tracking", WINDOW_NORMAL);

	video.release();
	destroyAllWindows();

	return 0;
}
