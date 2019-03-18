#include "FaceHelper.h"
#include <iostream>

const Size FaceHelper::standardSize(150, 150);
const Point FaceHelper::standardLeftEye(45, 60);
const Point FaceHelper::standardRightEye(104, 60);

/*
const Size FileParser::standardSize(75, 75);
const Point FileParser::standardLeftEye(21, 30);
const Point FileParser::standardRightEye(52, 30);
*/

cv::Size FaceHelper::getStandardSize()
{
	return FaceHelper::standardSize;
}

FaceHelper::FaceHelper()
{
	if(!eyeClassifier.load("haar/haarcascade_eye_tree_eyeglasses.xml") || !(faceClassifier.load("haar/haarcascade_frontalface_alt.xml"))) {
		cout << "Load haarcascade_mcs_eye.xml failed!" << endl;
		return;
	}
}

FaceHelper::~FaceHelper()
{

}

void FaceHelper::alignFace(cv::Mat& src)
{
	Mat image_gray;
	cvtColor(src, image_gray, CV_BGR2GRAY);
	equalizeHist(image_gray, image_gray);

	vector<Rect> eyeRect;
	eyeClassifier.detectMultiScale(image_gray, eyeRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(15, 15));
	Point2f eye1, eye2;
	Point2f rightEye;
	Point2f leftEye;
	Point2f middle;

	Mat temp;
	if(eyeRect.size() == 2) {
		eye1 = Point2f((float)eyeRect[0].x + eyeRect[0].width / 2, (float)eyeRect[0].y + eyeRect[0].height / 2);
		eye2 = Point2f((float)eyeRect[1].x + eyeRect[1].width / 2, (float)eyeRect[1].y + eyeRect[1].height / 2);

		if(eye1.x > eye2.x) {
			rightEye = eye1;
			leftEye = eye2;
		} else {
			leftEye = eye1;
			rightEye = eye2;
		}

		middle = (leftEye + rightEye) / 2.0f;
		assert(fabs(leftEye.x - rightEye.x) > 1e-5f);
		float slope = (rightEye.y - leftEye.y) / (rightEye.x - leftEye.x);
		float radial = atan(slope);
		float PI = (float)acos(-1.0f);
		Mat eyeRotation = getRotationMatrix2D(middle, radial / PI * 180.0f, 1.0);
		warpAffine(image_gray, temp, eyeRotation, image_gray.size());
		image_gray = temp;

		//imshow("rotation", image_gray);
	}

	eyeRect.clear();
	eyeClassifier.detectMultiScale(image_gray, eyeRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(15, 15));

	if(eyeRect.size() == 2) {
		eye1 = Point2f((float)eyeRect[0].x + eyeRect[0].width / 2, (float)eyeRect[0].y + eyeRect[0].height / 2);
		eye2 = Point2f((float)eyeRect[1].x + eyeRect[1].width / 2, (float)eyeRect[1].y + eyeRect[1].height / 2);

		if(eye1.x > eye2.x) {
			rightEye = eye1;
			leftEye = eye2;
		} else {
			leftEye = eye1;
			rightEye = eye2;
		}
		// scale before facing
		float ratio = (rightEye.x - leftEye.x) / (float)(FaceHelper::standardRightEye.x - FaceHelper::standardLeftEye.x);
		middle = (leftEye + rightEye) / 2.0f;
		resize(image_gray, temp, cv::Size2f(image_gray.size()) / ratio, middle.x, middle.y);
		image_gray = temp;

		//imshow("scale1", image_gray);
	}
	// face
	vector<Rect> faceRect;
	faceClassifier.detectMultiScale(image_gray, faceRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	// Make sure the object is completely within the image, in case it was on a border.
	for(int i = 0; i < (int)faceRect.size(); i++) {
		if(faceRect[i].x < 0)
			faceRect[i].x = 0;
		if(faceRect[i].y < 0)
			faceRect[i].y = 0;
		if(faceRect[i].x + faceRect[i].width > image_gray.cols)
			faceRect[i].x = image_gray.cols - faceRect[i].width;
		if(faceRect[i].y + faceRect[i].height > image_gray.rows)
			faceRect[i].y = image_gray.rows - faceRect[i].height;
	}
	image_gray = image_gray(faceRect[0]);

	resize(image_gray, temp, FaceHelper::standardSize);
	image_gray = temp;
	//imshow("face", image_gray);
	//waitKey();
	/*
	eyeRect.clear();
	eyeClassifier.detectMultiScale(image_gray, eyeRect, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	eye1 = Point2f(eyeRect[0].x + eyeRect[0].width / 2, eyeRect[0].y + eyeRect[0].height / 2);
	eye2 = Point2f(eyeRect[1].x + eyeRect[1].width / 2, eyeRect[1].y + eyeRect[1].height / 2);
	if(eye1.x > eye2.x) {
	rightEye = eye1;
	leftEye = eye2;
	} else {
	leftEye = eye1;
	rightEye = eye2;
	}
	int deltaY = FileParser::standardLeftEye.y - leftEye.y;
	temp = Mat::zeros(image_gray.size(), image_gray.type());
	if(deltaY > 1) {
	for(int i = 0; i < image_gray.rows - deltaY; i++) {
	for(int j = 0; j < image_gray.cols; j++)	{
	temp.at<uchar>(i + deltaY, j) = image_gray.at<uchar>(i, j);
	}
	}
	} else if(deltaY < -1) {
	for(int i = 0 - deltaY; i < image_gray.rows; i++){
	for(int j = 0; j < image_gray.cols; j++){
	temp.at<uchar>(i + deltaY, j) = image_gray.at<uchar>(i, j);
	}
	}
	}
	image_gray = temp;
	image_gray.at<uchar>(i, j)
	*/
	src = image_gray;
}