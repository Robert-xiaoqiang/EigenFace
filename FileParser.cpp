#include "FileParser.h"
#include "FaceHelper.h"
#include <filesystem>
#include <iostream>

FileParser::FileParser(const string& path):
	imagesMap(),
	names()
{
	FaceHelper fh;
	for(const auto& entry : std::experimental::filesystem::directory_iterator(path)) {
		string fileName = entry.path().string();
		string exposed = entry.path().filename().string();
		auto temp = exposed.find('.');

		if(temp == std::string::npos) continue;

		string name = exposed.substr(0, temp);
		try {
			if(names.find(name) == names.end()) {
				names.insert(name);
				Mat temp = imread(fileName);
				fh.alignFace(temp);
				deque<Mat> nameImages(1, temp);
				imagesMap.insert(std::make_pair(name, nameImages));
			} else {
				Mat temp = imread(fileName);
				fh.alignFace(temp);
				deque<Mat> nameImages(1, temp);
				imagesMap[name].push_back(temp);
			}
		} catch(const std::out_of_range& e) {
			cout << fileName << endl;
		}
	}
}

FileParser::~FileParser()
{

}

const std::deque<cv::Mat>& FileParser::operator[](const std::string& name) const
{
	auto pos = imagesMap.find(name);
	assert(pos != imagesMap.end());

	return pos->second;
}

std::deque<cv::Mat>& FileParser::operator[](const std::string& name)
{
	return imagesMap[name];
}

const std::unordered_set<std::string>& FileParser::getNames() const
{
	return names;
}
