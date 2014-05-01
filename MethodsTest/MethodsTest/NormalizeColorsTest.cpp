
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Methods.h"

using namespace std;
using namespace cv;

// Constants
const int WIDTH = 480;
const int HEIGHT = 640;

// Global variables
Mat frame;

int main()
{
	// Uniform images
	Mat mZeros = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(0.0,0.0,0.0));
	Mat mOnes = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(1.0,1.0,1.0));
	Mat mUniform = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(1.0/3,1.0/3,1.0/8));
	
	// Real image
	Mat rgb = imread("rgb.png");
	Mat rgbF;
	rgb.convertTo(rgbF,CV_32FC3);
	rgbF /= 255.0;

	Mat mNormalized = normalizeColors(rgbF);
	
	while(0)
	{
		imshow("Matrix image",rgbF);
		imshow("Matrix image normalized",mNormalized);
		waitKey(30);
	}
	
	// Video
	VideoCapture video("D:\MyVideo2.avi");
	
	Mat frameF;
	
	for (;;)
	{
		video >> frame;
		imshow("Main video",frame);
		
		frame.convertTo(frameF,CV_32FC3);
		
		//imshow("Normalized video",normalizeColors(frameF));
		waitKey(30);
	}

	return 0;
}
