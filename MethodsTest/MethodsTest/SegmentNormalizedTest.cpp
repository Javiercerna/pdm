/*
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Methods.h"

using namespace cv;
using namespace std;

// Constants
const int WIDTH = 480;
const int HEIGHT = 640;

// Global variables
Mat frame;
int option = 1;

int main()
{
	if (option == 1)
	{
		// Camera taken image
		Mat rgb = imread("rgb.png");

		Mat mNormalized = normalizeColors(rgb);
		
		//Scalar color1(20,20,130);
		//Scalar tolerance1(20,20,30);
		Point p(0,0);
		while(true)
		{
			imshow("Matrix image",rgb);
			imshow("Matrix image normalized",mNormalized);

			setMouseCallback("Matrix image normalized",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				showColorFromClick(mNormalized,p);
				p.x = 0;
				p.y = 0;
			}

			waitKey(30);
		}

	} else if (option == 2)
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