#ifndef FACEHELPER_H_
#define FACEHELPER_H_

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <string>
#include <map>
#include <deque>
#include <unordered_set>

using namespace cv;
using namespace std;

class FaceHelper {
public:
	FaceHelper();
	virtual ~FaceHelper();
	void alignFace(cv::Mat& src);
	static cv::Size getStandardSize();
private:
	CascadeClassifier eyeClassifier;
	CascadeClassifier faceClassifier;

	const static Size standardSize;
	const static Point standardLeftEye;
	const static Point standardRightEye;
};

#endif

