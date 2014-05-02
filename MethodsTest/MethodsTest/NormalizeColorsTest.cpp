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
		Mat mZeros = Mat(WIDTH,HEIGHT,CV_16UC3,Scalar(0,0,0));
		Mat mOnes = Mat(WIDTH,HEIGHT,CV_16UC3,Scalar(255,255,255));
		Mat mUniform = Mat(WIDTH,HEIGHT,CV_16UC3,Scalar(255/3,255/3,255/8));

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
		
		Mat mNormalized = normalizeColors(rgb);
	
		while(true)
		{
			imshow("Matrix image",rgb);
			imshow("Matrix image normalized",mNormalized);
			waitKey(30);
		}

	} else if (option == 3)
	{
		// Video
		VideoCapture video("D:\MyVideo2.avi");

		Mat normalized;
		for (;;)
		{
			video >> frame;
			
			if (frame.empty())
			{
				break;
			}

			imshow("Main video",frame);
		
			normalized = normalizeColors(frame);
			imshow("Normalized video",normalized);
			waitKey(10);
		}
	}

	return 0;
}
*/