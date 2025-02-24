#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


int main() {
	Mat image = imread("./image.jpg");

	if (image.empty()) {
			cout << "Ошибка: не удалось загрузить изображение" << endl;
			return -1;
	}

	// Прямоугольник
	rectangle(image, Point(50, 50), Point(200, 200), Scalar(255, 0, 0), 3);

	// Окружность
	circle(image, Point(350, 150), 100, Scalar(0, 0, 255), 3);

	// Линия
	line(image, Point(50, 300), Point(450, 300), Scalar(0, 255, 0), 2);

	imshow("Geometric Shapes", image);
	waitKey(0);

	return 0;
}
