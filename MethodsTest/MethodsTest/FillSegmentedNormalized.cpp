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
Mat segmented;
int option = 2;

int main()
{
	if (option == 1)
	{
		// Camera taken image
		Mat rgb = imread("rgb.png");
		Mat mNormalized = normalizeColors(rgb);
		
		Scalar color1(25,40,190); 
		Scalar tolerance1(25,20,30);

		Scalar color1HSV(5,220,180);
		Scalar tolerance1HSV(5,40,40);

		cvtColor(mNormalized,mNormalized,CV_BGR2HSV);
		
		/*
		Mat shirt = imread("azul2.png");
		mNormalized = normalizeColors(shirt);
		
		Scalar color2(210,120,70);
		Scalar tolerance2(70,60,40);

		Scalar color2HSV(110,170,205);
		Scalar tolerance2HSV(10,55,55);
		
		cvtColor(mNormalized,mNormalized,CV_BGR2HSV);
		
		Point p(0,0);
		while(true)
		{
			//imshow("Matrix image",rgb);
			//imshow("Matrix image normalized",mNormalized);

			setMouseCallback("Matrix image normalized",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				showColorFromClick(mNormalized,p);
				p.x = 0;
				p.y = 0;
			}

			segmented = segmentColor(mNormalized,color1HSV,tolerance1HSV);
			//drawFilledMoments(segmented,rgb,500,tolerance1);
			imshow("Matrix image",rgb);
			imshow("Segmented image",segmented);
			waitKey(30);
		}

	} else if (option == 2)
	{
		// Video
		VideoCapture video("D:\MyVideo2.avi");
		char key;
		Mat normalized;
		
		Scalar colorHSV(110,225,210);
		Scalar toleranceHSV(13,60,50);

		//Scalar tolerance(10,50,0);
		Point p(0,0);

		for (;;)
		{
			video >> frame;

			if (frame.empty())
			{
				break;
			}

			normalized = normalizeColors(frame);

			cvtColor(normalized,frameHSV,CV_BGR2HSV);
			
			setMouseCallback("Segmented video",mouseHandler,(void*)(&p));
			if (p.x != 0 && p.y != 0)
			{
				showColorFromClick(frameHSV,p);
				p.x = 0;
				p.y = 0;
			}

			segmented = segmentColor(frameHSV,colorHSV,toleranceHSV);
			
			erode(segmented,segmented,STREL);
			dilate(segmented,segmented,STREL);

			drawFilledMoments(segmented,frameHSV,2000,toleranceHSV);
			
			imshow("Segmented video",segmented);
			imshow("Normalized video",normalized);
			imshow("Main video",frameHSV);
			key = waitKey(1);
			if (key == 'p')
			{
				break;
			}
		}

		while(true)
		{
			if (!frame.empty())
			{
				imshow("Segmented video",frameHSV);
				setMouseCallback("Segmented video",mouseHandler,(void*)(&p));
				if (p.x != 0 && p.y != 0)
				{
					showColorFromClick(frameHSV,p);
					p.x = 0;
					p.y = 0;
				}
				waitKey(30);
			}
		}

	}

	return 0;
}
*/