#include <filesystem>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

namespace fs = filesystem;

struct SampleData {
	string label;
	Mat image;
	vector<KeyPoint> keypoints;
	Mat descriptors;
};

vector<DMatch> ratioTest(const vector<vector<DMatch>>& matches, double ratio = 0.75) {
	vector<DMatch> goodMatches;

	for (const auto& match : matches) {
		if (match.size() >= 2 && match[0].distance < ratio * match[1].distance) goodMatches.push_back(match[0]);
	}

	return goodMatches;
}

bool loadImages(const string& directory, vector<SampleData>& samples, Ptr<SIFT> detector) {
	if (!fs::exists(directory)) {
		cerr << "Error: Directory '" << directory << "' not found." << endl;
		return false;
	}

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.path().extension() == ".png") {
			SampleData sample;
			sample.label = entry.path().stem().string();
			sample.image = imread(entry.path().string());

			if (sample.image.empty()) {
				cerr << "Warning: Could not load image: " << entry.path() << endl;
				continue;
			}

			detector->detectAndCompute(sample.image, noArray(), sample.keypoints, sample.descriptors);
			samples.push_back(sample);
		}
	}

	return !samples.empty();
}

void drawDetection(Mat& output_image, const SampleData& sample, const vector<Point2f>& target_corners) {
	for (size_t i = 0; i < 4; ++i) {
		line(
			output_image,
			target_corners[i],
			target_corners[(i + 1) % 4],
			Scalar(255, 0, 0),
			2
		);
	}

	Point2f center(0, 0);
	for (const auto& corner : target_corners) center += corner;
	
	center /= static_cast<float>(target_corners.size());

	putText(
		output_image,
		sample.label,
		center - Point2f(50, 0),
		FONT_HERSHEY_DUPLEX,
		0.8,
		Scalar(0, 0, 255),
		2
	);
}

int main() {
	Ptr<SIFT> sift = SIFT::create();

	BFMatcher matcher(NORM_L2);
	vector<SampleData> samples;

	string dir = "./cards";
	if (!loadImages(dir, samples, sift)) {
		cout << "Error: No valid reference images loaded." << endl;
		return -1;
	}

	Mat target = imread("./image.png");
	if (target.empty()) {
		cout << "Error: Could not load target image." << endl;
		return -1;
	}

	vector<KeyPoint> keypoints;
	Mat descriptors;
	sift->detectAndCompute(target, noArray(), keypoints, descriptors);

	cout << "Target image keypoints detected: " << keypoints.size() << endl;

	Mat result = target.clone();
	bool goodMatches = false;

	for (const auto& sample : samples) {
		if (sample.descriptors.empty() || descriptors.empty()) {
			cout << "Skipping " << sample.label << ": No valid descriptors" << endl;
			continue;
		}

		vector<vector<DMatch>> matches;
		matcher.knnMatch(sample.descriptors, descriptors, matches, 2);

		auto goodImageMatches = ratioTest(matches);
		cout << sample.label << " matches found: " << goodImageMatches.size() << endl;

		if (goodImageMatches.size() < 4) {
			cout << sample.label << ": Insufficient matches for homography" << endl;
			continue;
		}

		vector<Point2f> samplePoints, targetPoints;
		for (const auto& match : goodImageMatches) {
			samplePoints.push_back(sample.keypoints[match.queryIdx].pt);
			targetPoints.push_back(keypoints[match.trainIdx].pt);
		}

		Mat homography = findHomography(samplePoints, targetPoints, RANSAC);
		if (homography.empty()) {
			cout << sample.label << ": Homography calculation failed" << endl;
			continue;
		}

		vector<Point2f> sample_corners = {
			{0, 0},
			{static_cast<float>(sample.image.cols), 0},
			{static_cast<float>(sample.image.cols), static_cast<float>(sample.image.rows)},
			{0, static_cast<float>(sample.image.rows)}
		};
		vector<Point2f> target_corners;
		perspectiveTransform(sample_corners, target_corners, homography);

		double contour_area = contourArea(target_corners);
		cout << sample.label << " contour area: " << contour_area << endl;

		if (contour_area < 1000.0) {
			cout << sample.label << ": Contour area too small" << endl;
			continue;
		}

		drawDetection(result, sample, target_corners);
		goodMatches = true;
	}

	if (!goodMatches) {
		cout << "No valid matches found for any reference sample." << endl;
	}

	imshow("Detected Cards", result);
	waitKey(0);
	return 0;
}
