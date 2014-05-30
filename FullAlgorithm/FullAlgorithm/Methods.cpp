#include <cv.h>
#include "Methods.h"
#include <highgui.h>
#include <iostream>
#include <math.h>

using namespace cv;
using namespace std;

/***
NormalizeColors: Returns an image with normalized 3-channel colors.
Input: 3-channel color image m.
Output: 3-channel color image. 
***/
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

/***
SegmentColor: Returns a binary image which corresponds to the segmented color.
Input: 3-channel color (inputImage).
	   Color to be detected (color).
	   Allowed color variation (tolerance).
Output: Binary image.
***/
Mat segmentColor(Mat inputImage, Scalar color, Scalar tolerance) 
{
	Mat outputImage;
	
	Scalar zero(0,0,0);
	Scalar one(255,255,255);
	
	inRange(inputImage,max(zero,color-tolerance),min(one,color+tolerance),outputImage);
	
	return outputImage;
}

/***
Max: Returns the greater of two scalars a and b.
***/
Scalar max(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = max(a[0],b[0]);
	out[1] = max(a[1],b[1]);
	out[2] = max(a[2],b[2]);
	return out;
}

/***
Min: Returns the lower of two scalars a and b.
***/
Scalar min(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = min(a[0],b[0]);
	out[1] = min(a[1],b[1]);
	out[2] = min(a[2],b[2]);
	return out;
}

/***
DrawMoments: Draws a rectangle based on the binary image given and their calculated moments.
			 It then returns the centroids and the area.
Input: Segmented binary image (imageBW).
	   Image in which to draw the rectangle (original).
	   Minimum area considered to be the player's shirt (minArea).
Output: Centroids and area in a vector [centroid_X,centroid_Y,area]
***/
vector<double> drawMoments(Mat imageBW, Mat &original,double minArea) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<double> player = vector<double>(3);

	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 
	vector<Rect> boundRect;

	for(int i = 0; i < contours.size(); i++)
	{ 
		mu.push_back(moments(contours[i], false)); 
	}

	//Centroids 
	vector<double> cX;
	vector<double> cY;
	vector<double> area;

	for (int i = 0; i < contours.size(); i++)
	{
		if (mu[i].m00 > minArea) 
		{
			boundRect.push_back(boundingRect(Mat(contours[i])));
			cX.push_back(mu[i].m10/mu[i].m00);
			cY.push_back(mu[i].m01/mu[i].m00);
			area.push_back(mu[i].m00);
		}
	}
	
	int index = maxAreaIndex(area);

	if (index != -1) 
	{
		rectangle(original, boundRect[index], Scalar(255,0,0), 2, 8, 0 );
		player[0] = cX[index];
		player[1] = cY[index];
		player[2] = area[index];
	}
	
	return player;
}

/***
DrawMoments: Draws rectangles based on the binary image given and their calculated moments.
			 It then returns the centroids and the area of each armband.
Input: Segmented binary image (imageBW).
	   Image in which to draw the rectangle (original).
	   Minimum area considered to be the player's armbands (minArea).
	   Detected player (p).
Output: Centroids and area of each armband in a vector 
		[centroid_X1,centroid_Y1,area1,centroid_X2,centroid_Y2,area2].
***/
vector<double> drawMoments(Mat imageBW, Mat &original,double minArea, Player p) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<double> hands = vector<double>(6);

	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 
	vector<Rect> boundRect;

	for(int i = 0; i < contours.size(); i++)
	{ 
		mu.push_back(moments(contours[i], false)); 
	}

	//Centroids 
	vector<double> cX;
	vector<double> cY;
	vector<double> area;
	vector<double> theta;

	for (int i = 0; i < contours.size(); i++)
	{
		//double distance = getDistance(Point(mu[i].m10/mu[i].m00,mu[i].m01/mu[i].m00),Point(p.getCX(),p.getCY())); 
		if (mu[i].m00 > minArea && mu[i].m01/mu[i].m00 < p.getCY() && mu[i].m00 < p.getArea()/10) 
		{
			// && distance < sqrt(p.getArea()) 
			boundRect.push_back(boundingRect(Mat(contours[i])));
			cX.push_back(mu[i].m10/mu[i].m00);
			cY.push_back(mu[i].m01/mu[i].m00);
			area.push_back(mu[i].m00);
			//theta.push_back(0.5 * atan2((2 * mu[i].mu11),(mu[i].mu20 -  mu[i].mu02)));
		}
	}
	
	int index1 = maxAreaIndex(area);
	int index2 = -1;

	if (index1 != -1) 
	{
		for (int i = 0; i < area.size(); i++)
		{
			//double distance1 = getDistance(Point(cX[index1],cY[index1]),Point(p.getCX(),cY[index1]));
			//double distance2 = getDistance(Point(cX[i],cY[i]),Point(p.getCX(),cY[i]));

			if (abs(area[i] - area[index1]) < 150 && i != index1 && abs(cX[i] - cX[index1] > 100))
			{
				// && abs(distance1-distance2) < 10 && abs(distance1-distance2) > 5
				index2 = i;
			}
		}
		//circle(original,Point(cX[i],cY[i]),20, Scalar(0,0,255),-1);
		rectangle(original, boundRect[index1], Scalar(0,0,255), 2, 8, 0 );
		//line(original,Point(cX[index1],cY[index1]),Point(cX[index1] + 50*cos(theta[index1]),cY[index1] + 50*sin(theta[index1])),Scalar(255,0,0),10);
		hands[0] = cX[index1];
		hands[1] = cY[index1];
		hands[2] = area[index1];
		if (index2 != -1)
		{
			rectangle(original, boundRect[index2], Scalar(0,0,255), 2, 8, 0 );
			//line(original,Point(cX[index2],cY[index2]),Point(cX[index2] + 50*cos(theta[index2]),cY[index2] + 50*sin(theta[index2])),Scalar(255,0,0),10);	
			hands[3] = cX[index2];
			hands[4] = cY[index2];
			hands[5] = area[index2];
		}
	}

	return hands;
}

/***
MaxAreaIndex: Returns the greatest area's index from a given area vector.
Input: Area vector (areas).
Output: Index of the greatest area in the vector.
***/
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

/***
MouseHandler 
***/
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

/***
ShowColorFromClick
***/
void showColorFromClick(Mat m,Point p)
{
	vector<Mat> channels;

	split(m,channels);

	int c1 = (int)channels[0].at<uchar>(p);
	int c2 = (int)channels[1].at<uchar>(p);
	int	c3 = (int)channels[2].at<uchar>(p);
	
	cout << "[" << c1 << "," << c2 << "," << c3 << "]\n";
}

/***
GetDistance: Returns the distance between two points.
Input: Points from which the distance is going to be calculated (p1,p2).
Output: Distance calculated.
***/
double getDistance(Point p1,Point p2)
{
	double dist = (p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y);
	return sqrt(dist);
}