#define _CRT_SECURE_NO_WARNINGS
#ifndef FILEPARSER_H_
#define FILEPARSER_H_

#include <string>
#include <map>
#include <deque>
#include <unordered_set>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"
// #include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class FileParser {
public:
	FileParser(const std::string& path);
	virtual ~FileParser();
	const std::deque<cv::Mat>& operator[](const std::string& name) const;
	std::deque<cv::Mat>& operator[](const std::string& name);
	const std::unordered_set<std::string>& getNames() const;

private:
	std::map<std::string, std::deque<cv::Mat>> imagesMap;
	std::unordered_set<std::string> names;
};

#endif
