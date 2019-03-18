#define _CRT_SECURE_NO_WARNINGS
// #include <opencv2/opencv.hpp>
#include <vector>
#include <cassert>
#include <iostream>
#include <cmath>
#include "FileParser.h"
#include "EigenGenerator.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
	if(argc != 4) {
		std::cout << "TrainMain #faces(10 base presentation) modelFileName faceLibName" << std::endl;
	}
	FileParser fp(argv[3]);
	EigenGenerator eg(std::stoi(argv[1])); // PC# # EigenFace
	auto a = fp.getNames();
	std::deque<cv::Mat> all;
	for(const auto& n : a) {
		auto images = fp[n];
		all.insert(all.end(), images.begin(), images.end());
	}
	eg.insertFace(all);
	// dir name
	// model file name
	eg.saveAll(argv[2]);
	return 0;
}
