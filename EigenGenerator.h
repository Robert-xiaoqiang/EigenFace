#define _CRT_SECURE_NO_WARNINGS
#ifndef EIGENGENERATOR_H_
#define EIGENGENERATOR_H_

#include <map>
#include <string>
#include <deque>
#include <tuple>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

// #include <opencv2/opencv.hpp>

class EigenGenerator {
public:
	EigenGenerator(int k);
	virtual ~EigenGenerator();
	void insertFace(const std::deque<cv::Mat>& images);
	void saveAll(const std::string& dirName) const;
private:
	// name -> (mean, eigenface kth)
	// make sure that column vector
	// std::map<std::string, std::tuple<cv::Mat, cv::Mat>> eigensMap;
	cv::Mat eigens;
	cv::Mat mean;
	int k;
};

#endif
