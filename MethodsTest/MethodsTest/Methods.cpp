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
	channels[0] = (int)255*channels[0]/sum;
	channels[1] = (int)255*channels[1]/sum;
	channels[2] = (int)255*channels[2]/sum;
	
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
		if (mu[i].m01/mu[i].m00 > 200 && mu[i].m00 > minArea) // > 200 
		{
			boundRect.push_back(boundingRect(Mat(contours[i])));
			cX.push_back(mu[i].m10/mu[i].m00);
			cY.push_back(mu[i].m01/mu[i].m00);
			area.push_back(mu[i].m00);
		}
	}
	
	for (int i = 0; i < area.size(); i++)
	{
		//circle(original,Point(cX[i],cY[i]),20, Scalar(0,0,255),-1);
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

void drawFilledMoments(Mat imageBW, Mat &original,double minArea,Scalar tolerance) 
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
		if (mu[i].m01/mu[i].m00 > 200 && mu[i].m00 > minArea) // > 200 
		{
			boundRect.push_back(boundingRect(Mat(contours[i])));
			cX.push_back(mu[i].m10/mu[i].m00);
			cY.push_back(mu[i].m01/mu[i].m00);
			area.push_back(mu[i].m00);
		}
	}
	
	Mat mask = Mat::zeros(imageBW.rows,imageBW.cols,imageBW.type());
	
	for (int i = 0; i < area.size(); i++)
	{
		mask = mask | fillMask(original,Point(cX[i],cY[i]),tolerance,tolerance);
		//circle(original,Point(cX[i],cY[i]),20, Scalar(0,0,255),-1);
		//rectangle(original, boundRect[i], Scalar(255,0,0), 2, 8, 0 );
	}
	
	drawMoments(mask,original,minArea);
	imshow("Mask",mask);
	
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

void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
    case CV_EVENT_LBUTTONUP:
		cv::Point* p = (cv::Point*)param;
		p->x = x;
		p->y = y;
        break;
    }
}

void showColorFromClick(Mat m,Point p)
{
	vector<Mat> channels;

	split(m,channels);

	int c1 = (int)channels[0].at<uchar>(p);
	int c2 = (int)channels[1].at<uchar>(p);
	int	c3 = (int)channels[2].at<uchar>(p);
	
	cout << "[" << c1 << "," << c2 << "," << c3 << "]\n";
}

Mat fillMask(Mat frame,Point seed,Scalar maxLoDiff, Scalar maxHiDiff)
{
	int newMaskVal = 255;
	Scalar newVal = Scalar( 120, 120, 120 );
	Mat mask;

	int connectivity = 8;
	int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;
	Mat mask2 = Mat::zeros(frame.rows + 2, frame.cols + 2, CV_8UC1);

	floodFill(frame, mask2, seed, newVal, 0, maxLoDiff, maxHiDiff, flags);
	mask = mask2(Range(1, mask2.rows - 1), Range(1, mask2.cols - 1));
	
	return mask;
}
