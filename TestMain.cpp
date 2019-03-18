#include "EigenTester.h"
#include <filesystem>
#include <iostream>
#include <iomanip>

int main(int argc, char *argv[])
{
	if(argc != 4) {
		std::cout << "TestMain dirName(to recognize) modelFileName faceLibName" << std::endl;
	};
	EigenTester eigenTester(argv[2], argv[3]);
	int N = 0, hit = 0;
	std::cout << "y/Y for correct matching and\nn/N for wrong matching" << std::endl;
	for(const auto& entry : std::experimental::filesystem::directory_iterator(argv[1])) {
		if(std::experimental::filesystem::is_directory(entry.path())) {
			continue;
		}
		const std::string& fileName = entry.path().string();
		bool result = eigenTester.test(fileName);
		N++;
		hit += (int)result;
	}
	std::cout << hit << '/' << N << " with ratio "
		<< std::fixed << std::setprecision(2)
		<< 100.0 * static_cast<double>(hit) / N 
		<< '%' << std::endl;
	return 0;
}

