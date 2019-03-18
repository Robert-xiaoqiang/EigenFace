#ifndef EIGENTESTER_H_
#define EIGENTESTER_H_

#include <string>
#include <map>
#include <tuple>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "FaceHelper.h"
#include "FileParser.h"

class EigenTester {
public:
	EigenTester(const std::string& fileName, const std::string& dirName);
	virtual ~EigenTester();
	bool test(const std::string& fileName);
private:
	cv::Mat getCoordinate(const cv::Mat& img) const;
	cv::Mat eigens;
	cv::Mat eigenst64;
	cv::Mat eigens64;
	cv::Mat mean;
	cv::Mat mean64;
	FaceHelper fh;
	FileParser fp;
};

#endif

