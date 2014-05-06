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
const Mat STREL = getStructuringElement(MORPH_ELLIPSE, Size(7,7));

// Global variables
Mat frame;
Mat frameHSV;
Mat mask;
Mat segmented;
int option = 2;

int main()
{
	
	if (option == 1)
	{
		// Separated colors image
		Mat rgb = imread("rgbColor.png");
		
		Scalar maxLoDiff(20,20,20);
		Scalar maxHiDiff(20,20,20);

		Point p(0,0);
		while (true)
		{
			setMouseCallback("Image",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				mask = fillMask(rgb,p,maxLoDiff,maxHiDiff);
				p.x = 0;
				p.y = 0;
				imshow("Mask",mask);
			}
			imshow("Image",rgb);
			waitKey(30);
		}
	} else if (option == 2)
	{
		// Camera taken images
		Mat field = imread("rgb.png");

		cvtColor(field,field,CV_BGR2HSV);	
		
		Scalar color1(20,20,130);
		Scalar tolerance1(20,20,30);

		Scalar color1HSV(5,210,130);
		Scalar tolerance1HSV(5,30,30);
		
		Mat shirt = imread("azul2.png");
		
		Scalar color3(210,120,70);
		Scalar tolerance3(70,60,40);

		Scalar color3HSV(110,170,215);
		Scalar tolerance3HSV(10,55,55);

		//cvtColor(shirt,shirt,CV_BGR2HSV);
		Point p(0,0);
		while (true)
		{
			setMouseCallback("Image",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				mask = fillMask(shirt,p,tolerance3,tolerance3);
				p.x = 0;
				p.y = 0;
				imshow("Mask",mask);
			}
			imshow("Image",shirt);
			waitKey(30);	
		}
	} else if (option == 3)
	{
		// Video
		VideoCapture video("D:\MyVideo2.avi");
		char key;

		Scalar colorHSV(110,225,80);
		Scalar toleranceHSV(13,60,60);

		Point p(0,0);
		while(true)
		{
			video >> frame;
			
			if (frame.empty())
			{
				break;
			}
            
			cvtColor(frame,frameHSV,CV_BGR2HSV);
			setMouseCallback("Main video",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				mask = fillMask(frameHSV,p,toleranceHSV,toleranceHSV);
				p.x = 0;
				p.y = 0;
				imshow("Mask",mask);
			}
			
			imshow("Main video",frame);
			
			key = waitKey(30);
			if (key == 'p')
			{
				break;
			}
		}

		while(true)
		{
			if (!frame.empty())
			{
				imshow("Main video",frame);
				if (p.x != 0 && p.y != 0)
				{	
					mask = fillMask(frameHSV,p,toleranceHSV,toleranceHSV);
					p.x = 0;
					p.y = 0;
					imshow("Mask",mask);
				}
				waitKey(30);
			}
		}
	}

	return 0;
}
*/