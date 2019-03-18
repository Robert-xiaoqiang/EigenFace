#define _CRT_SECURE_NO_WARNINGS
#include "EigenGenerator.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

EigenGenerator::EigenGenerator(int k) :
	eigens(),
	k(k)
{
	
}

EigenGenerator::~EigenGenerator()
{

}

void EigenGenerator::insertFace(const std::deque<cv::Mat>& images)
{
	std::vector<cv::Mat> vecs;
	int width = images[0].cols;
	int height = images[0].rows;

	std::deque<cv::Mat> images64;
	std::transform(images.begin(), images.end(), std::back_inserter(images64), [](const auto& i) -> cv::Mat
	{
		cv::Mat temp = i.clone();
		temp.convertTo(temp, 6);

		return temp;
	});

	cv::Mat mean = cv::Mat::zeros(width * height, 1, 6);
	for(const auto& i : images64) {
		cv::Mat temp = i.reshape(0, width * height).clone();
		mean += temp;
		vecs.push_back(temp);
	}
	mean /= static_cast<double>(images.size());

	cv::Mat normalMean;
	cv::normalize(mean, normalMean, 0, 255, cv::NORM_MINMAX, CV_8UC1);

	cv::Mat buildAll;
	cv::hconcat(vecs, buildAll);

	cv::Mat fai = cv::Mat::zeros(width * height, images.size(), 6); // 64F
	for(int i = 0; i < vecs.size(); i++) {
		cv::Mat temp = vecs[i] - mean;
		temp.copyTo(fai.col(i));
	}

	cv::Mat cov;
	cv::calcCovarMatrix(buildAll, cov, mean, CV_COVAR_SCRAMBLED | CV_COVAR_COLS);
	cov /= static_cast<double>(images.size() - 1);

	cv::Mat eigenValues, eigenVectors;
	cv::eigen(cov, eigenValues, eigenVectors);

	cv::Mat faces = fai * eigenVectors;
	cv::Mat normalFaces = cv::Mat::zeros(faces.rows, faces.cols, CV_8UC1);

	cv::Mat tenFace = cv::Mat::zeros(height * 3, width * 5, CV_8UC1);

	for(int i = 0; i < faces.cols; i++) {
		cv::Mat face = faces.col(i);
		cv::Mat normalFace;
		cv::normalize(face, normalFace, 0, 255, cv::NORM_MINMAX, CV_8UC1);
		normalFace.copyTo(normalFaces.col(i));
		if(i < 10) {
			normalFace.clone().reshape(0, height).copyTo(tenFace(cv::Rect(width * (i % 5), (i / 5 + 1) * height, width, height)));
		}
	}
	// eigensMap.insert(std::make_pair(name, std::make_tuple(normalMean, normalFaces(cv::Rect(0, 0, k, height * width)))));
	
	// mean
	normalMean.clone().reshape(0, height).copyTo(tenFace(cv::Rect(2 * width, 0, width, height)));

	cv::imshow("10th eigen face", tenFace);
	cv::imwrite("eigens/" "tenth_eigenface.png", tenFace);
	eigens = normalFaces(cv::Rect(0, 0, k, height * width));
	this->mean = normalMean;
	cv::waitKey();
}

void EigenGenerator::saveAll(const std::string& modelFileName) const
{
	// cv::FileStorage fs(dirName + "/faces.model", cv::FileStorage::WRITE);
	cv::FileStorage fs(modelFileName, cv::FileStorage::WRITE);
	fs << "name" << "faces";
	fs << "mean" << mean;
	fs << "face" << eigens;
	fs.release();
}