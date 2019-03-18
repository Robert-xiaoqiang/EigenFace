#include "EigenTester.h"
#include "Fileparser.h"
#include <filesystem>
#include <iostream>

EigenTester::EigenTester(const std::string& fileName, const std::string& dirName):
	eigens(),
	fh(),
	fp(dirName)
{
	cv::FileStorage fs(fileName, cv::FileStorage::READ);
	std::string name;
	fs["name"] >> name;
	fs["mean"] >> mean;
	fs["face"] >> eigens;

	mean.convertTo(mean64, 6);

	eigens64 = eigens.clone();
	eigens64.convertTo(eigens64, 6);

	for(int i = 0; i < eigens64.cols; i++) {
		cv::Mat len2 = (eigens64.col(i).t() * eigens64.col(i));
		eigens64.col(i) = eigens64.col(i) / std::sqrt(len2.at<double>(0, 0));
	}

	eigenst64 = eigens64.t();
}

EigenTester::~EigenTester()
{

}

/**
 * img(width * height) gray
 * after alignment
 */
cv::Mat EigenTester::getCoordinate(const cv::Mat& img) const
{
	int width = FaceHelper::getStandardSize().width;
	int height = FaceHelper::getStandardSize().height;
	cv::Mat img1 = img.clone().reshape(0, height * width);

	img1.convertTo(img1, 6);
	cv::Mat dif = (img1 - mean64);

	return eigenst64 * dif;
}

bool EigenTester::test(const std::string& fileName)
{
	bool ret = false;
	int width = FaceHelper::getStandardSize().width;
	int height = FaceHelper::getStandardSize().height;

	cv::Mat result = cv::Mat::zeros(height, width * 4, CV_8UC1);

	cv::Mat src = cv::imread(fileName);
	cv::Mat srcGray, srcTemp;
	cv::resize(src, srcGray, cv::Size(width, height));
	cv::cvtColor(srcGray, srcTemp, CV_BGR2GRAY);
	srcTemp.copyTo(result(cv::Rect(0, 0, width, height)));

	fh.alignFace(src);
	cv::Mat coordinate = getCoordinate(src);

	cv::Mat build = eigens64 * coordinate + mean64;
	cv::Mat normalBuild;
	cv::normalize(build, normalBuild, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	normalBuild.reshape(0, height).copyTo(result(cv::Rect(width, 0, width, height)));

	cv::Mat src64;
	src.convertTo(src64, 6);
	cv::Mat srcAdditional = src64.reshape(0, width * height) + build;
	cv::Mat normalSrcAdditional;
	cv::normalize(srcAdditional, normalSrcAdditional, 0, 255, cv::NORM_MINMAX, CV_8UC1);
	normalSrcAdditional.reshape(0, height).copyTo(result(cv::Rect(width * 2, 0, width, height)));

	std::string minName = *(fp.getNames().begin());

	/**
	 * fp(name, width * height) normalized
	 * not colume vector
	 */
	cv::Mat similar = fp[minName][0];
	double minDistance = cv::norm(getCoordinate(similar), coordinate, NORM_L2);
	for(const auto& i : fp.getNames()) {
		for(const auto& j : fp[i]) {
			double distance = cv::norm(getCoordinate(j), coordinate, NORM_L2);
			if(distance < minDistance) {
				minDistance = distance;
				similar = j;
				minName = i;
			}
		}
	}
	similar.copyTo(result(cv::Rect(width * 3, 0, width, height)));
	cv::putText(result,
				minName,
				cv::Point2i{ 20, 30 },
				cv::FONT_HERSHEY_PLAIN,
				2.0,
				cv::Scalar(255),
				2,
				false);
	cv::destroyAllWindows();
	cv::imshow(fileName + "_result", result);
	cv::imwrite(fileName + "_result.png", result);
	while(true) {
		int key = cv::waitKey(10);
		if(key == 'y' || key == 'Y') {
			ret = true;
			break;
		} else if(key == 'n' || key == 'N') {
			ret = false;
			break;
		}
	}
	return ret;
}

