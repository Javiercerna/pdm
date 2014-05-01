/*
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
int option = 3;

int main()
{
	if (option == 1)
	{
		// Uniform images
		Mat mZeros = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(0.0,0.0,0.0));
		Mat mOnes = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(1.0,1.0,1.0));
		Mat mUniform = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(1.0/3,1.0/3,1.0/8));

		Mat mNormalized = normalizeColors(mOnes);
	
		while (true)
		{
			imshow("Matrix image",mOnes);
			imshow("Matrix image normalized",mNormalized);
			waitKey(30);
		}

	} else if (option == 2)
	{
		// Real image
		Mat rgb = imread("rgb.png");
		Mat rgbF;
		rgb.convertTo(rgbF,CV_32FC3);
		rgbF /= 255.0;

		Mat mNormalized = normalizeColors(rgbF);
	
		while(true)
		{
			imshow("Matrix image",rgbF);
			imshow("Matrix image normalized",mNormalized);
			waitKey(30);
		}

	} else if (option == 3)
	{
		// Video
		VideoCapture video("D:\Video.mov");

		Mat frameF;
		Mat normalized;
		for (;;)
		{
			video >> frame;
			imshow("Main video",frame);
		
			frame.convertTo(frameF,CV_32FC3);
			
			normalized = normalizeColors(frameF);
			imshow("Normalized video",normalized);
			waitKey(10);
		}
	}

	return 0;
}
*/