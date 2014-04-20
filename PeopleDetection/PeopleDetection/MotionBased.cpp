#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace cv;

// Global variables
Mat colorFrame;
Mat foreground;
Ptr<BackgroundSubtractor> pMOG;
int blurSize = 5;

// Methods
void detectMotion();
void processForeground();
void drawMoments(Mat imageBW, Mat &original);
int maxAreaIndex(vector<double> areas);
void keyPressed(char k);

int main()
{
	VideoCapture cam(0);
	pMOG = new BackgroundSubtractorMOG(3, 3, 0.4,1.0);

	char key;
	while (true)
	{
		cam >> colorFrame;
		detectMotion();
		imshow("Main camera",colorFrame);

		key = waitKey(30);
		//keyPressed(key);
	}
	return 0;
}

void detectMotion() 
{	
	Mat maskedFrame;
	blur(colorFrame,colorFrame,Point(blurSize,blurSize));
	pMOG->operator()(colorFrame, foreground);

	processForeground();

	colorFrame.copyTo(maskedFrame,foreground);
	imshow("FG Mask MOG", foreground);
	imshow("Color frame masked",maskedFrame);
}

void processForeground()
{
	drawMoments(foreground,colorFrame);
}

void drawMoments(Mat imageBW, Mat &original) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 

	for( int i = 0; i < contours.size(); i++ )
	{ 
		mu.push_back(moments(contours[i], false)); 
	}

	//Centroids 
	vector<double> cX;
	vector<double> cY;
	vector<double> area;

	for (int i = 0; i < contours.size(); i++)
	{
		cX.push_back(mu[i].m10/mu[i].m00);
		cY.push_back(mu[i].m01/mu[i].m00);
		area.push_back(mu[i].m00);
	}

	int maxIndex = maxAreaIndex(area);

	vector<double> centroid;
	centroid.push_back(0);
	centroid.push_back(0);

	if (maxIndex != -1) 
	{
		cout << area[maxIndex] << endl;
		circle(original,Point(cX[maxIndex],cY[maxIndex]),20, Scalar(0,0,255),-1);
	}
	
	cout << "New loop" << endl;
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

/*
void keyPressed(char k)
{
	switch (k)
	{
	case '+':
		blurSize++;
		cout << blurSize << endl;
		break;
	case '-':
		if (blurSize > 0)
		{
			blurSize--;
			cout << blurSize << endl;
		}
	}
}
*/

