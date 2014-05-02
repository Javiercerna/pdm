#include <cv.h>
#include "Methods.h"
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat normalizeColors(Mat m)
{
	vector<Mat> channels;
	Mat output;
	Mat sum;

	split(m,channels);

	sum = channels[0] + channels[1] + channels[2];
	channels[0] = channels[0]/sum;
	channels[1] = channels[1]/sum;
	channels[2] = channels[2]/sum;
	
	merge(channels,output);

	sum.release();

	return output;
}

Mat segmentColor(Mat inputImage, Scalar color, Scalar tolerance) 
{
	Mat outputImage;
	
	Scalar zero(0,0,0);
	Scalar one(255,255,255);
	
	inRange(inputImage,max(zero,color-tolerance),min(one,color+tolerance),outputImage);
	
	return outputImage;
}

Scalar max(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = max(a[0],b[0]);
	out[1] = max(a[1],b[1]);
	out[2] = max(a[2],b[2]);
	return out;
}

Scalar min(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = min(a[0],b[0]);
	out[1] = min(a[1],b[1]);
	out[2] = min(a[2],b[2]);
	return out;
}

void drawMoments(Mat imageBW, Mat &original,double minArea) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	
	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 
	vector<Rect> boundRect;

	for(int i = 0; i < contours.size(); i++ )
	{ 
		mu.push_back(moments(contours[i], false)); 
	}

	//Centroids 
	vector<double> cX;
	vector<double> cY;
	vector<double> area;

	for (int i = 0; i < contours.size(); i++)
	{
		if (mu[i].m01/mu[i].m00 > 200 && mu[i].m00 > minArea) 
		{
			boundRect.push_back(boundingRect(Mat(contours[i])));
			cX.push_back(mu[i].m10/mu[i].m00);
			cY.push_back(mu[i].m01/mu[i].m00);
			area.push_back(mu[i].m00);
		}
	}
 
	for (int i = 0; i < area.size(); i++)
	{
		circle(original,Point(cX[i],cY[i]),20, Scalar(0,0,255),-1);
		rectangle(original, boundRect[i], Scalar(255,0,0), 2, 8, 0 );
	}

	/*
	int maxIndex = maxAreaIndex(area);
	
	if (maxIndex != -1 && area[maxIndex] > MIN_AREA) 
	{
		cout << area[maxIndex] << endl;

		object[0] = cX[maxIndex];
		object[1] = cY[maxIndex];
		object[2] = area[maxIndex];

		averageRectangle(boundRect[maxIndex]);
		circle(original,Point(cX[maxIndex],cY[maxIndex]),20, Scalar(0,0,255),-1);
		rectangle(original, boundingRectangle, Scalar(255,0,0), 2, 8, 0 );
	}
	*/
	
}

int maxAreaIndex(vector<double> areas)
{
	double maxArea = 0.0;
	int maxIndex = -1;

	for (int i = 0; i < areas.size(); i++)
	{
		if (areas[i] > maxArea)
		{
			maxArea = areas[i];
			maxIndex = i;
		}
	}

	return maxIndex;
}