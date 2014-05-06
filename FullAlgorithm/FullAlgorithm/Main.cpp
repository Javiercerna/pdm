#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Player.h"
#include "Methods.h"

using namespace std;
using namespace cv;

// Constants
const int WIDTH = 640;
const int HEIGHT = 480;
const int minArea = 0; // Minimum area considered
Scalar colorHSV(110,225,80); // Color detection parameters
Scalar toleranceHSV(13,60,60);	
const int upperBound = 134;
const int lowerBound = 463;
Rect rect = Rect(WIDTH/4,upperBound,WIDTH/2,lowerBound-upperBound); // Bounding rectangle
const Mat STREL = getStructuringElement(MORPH_ELLIPSE, Size(7,7)); // Erode and dilate strel

// Global variables
Player player = Player(); // Player + Hands
bool playerDetected = false;
Mat frame;
Mat ROI;

// Methods
void keyHandler(char k);
void detectPlayer();
void setPlayerAttributes(double cX, double cY, double area);
void trackPlayer();

int main()
{
	VideoCapture cam(0);
	cam.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);

	char key;

	while(true)
	{
		cam >> frame;
		ROI = frame(rect);

		if (!playerDetected)
		{
			detectPlayer();
			player.printPlayer();
		} else 
		{
			
			trackPlayer();
		}

		imshow("Main video",frame);
		imshow("ROI",ROI);
		key = waitKey(30);
		keyHandler(key);
	}

	return 0;
}

void keyHandler(char k)
{

}

void detectPlayer()
{
	Mat frameHSV;
	Mat segmented;
	vector<double> playerAttributes = vector<double>(3); // [cX,cY,area]

	cvtColor(ROI,frameHSV,CV_BGR2HSV);
	segmented = segmentColor(frameHSV,colorHSV,toleranceHSV);
			
	erode(segmented,segmented,STREL);
	dilate(segmented,segmented,STREL);

	imshow("Segmented",segmented);
	playerAttributes = drawMoments(segmented,ROI,minArea);

	setPlayerAttributes(playerAttributes[0],playerAttributes[1],playerAttributes[2]);
	frameHSV.release();
	segmented.release();
}


void setPlayerAttributes(double cX, double cY, double area)
{
	if (cX != 0.0 && cY != 0.0 && area != 0.0) {
		player.setCX(cX);
		player.setCY(cY);
		player.setArea(area);
		//playerDetected = true;
	}
}

void trackPlayer()
{

}