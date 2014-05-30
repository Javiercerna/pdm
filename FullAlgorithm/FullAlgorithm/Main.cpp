#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Methods.h"

using namespace std;
using namespace cv;

// Constants
const int WIDTH = 640;
const int HEIGHT = 480;
const int minArea = 100; // Minimum area considered
const int minHeight = 200;
Scalar colorHSV(42,165,200); // Color detection parameters
Scalar toleranceHSV(6,90,55);
Scalar armColorHSV(120,255,30);
Scalar armToleranceHSV(10,20,60);
const int upperBound = 134;
const int lowerBound = 463;
Rect rect = Rect(0,upperBound,WIDTH,lowerBound-upperBound); // Bounding rectangle
const Mat STREL = getStructuringElement(MORPH_ELLIPSE, Size(7,7)); // Erode and dilate strel

// Global variables
Player player = Player(); // Player + Hands
bool playerDetected = false;
bool handsDetected = false;
Mat frame;
Mat ROI;
KalmanFilter KF(4, 2, 0);
Mat_<float> measurement(2,1);

// Methods
void detectPlayer();
void setPlayerAttributes(vector<double> pAttrib);
void detectHands();
void setHandAttributes(vector<double> hAttrib);
void takeAction();
void checkBoundaries();

int main()
{
	// Camera parameters
	VideoCapture cam(0);
	cam.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);

	char key;

	// Kalman filter initialization
	KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,100,0,   0,1,0,100,  0,0,1,0,  0,0,0,1);
	measurement.setTo(Scalar(0));
	KF.statePre.at<float>(0) = 320;
	KF.statePre.at<float>(1) = 240;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
	setIdentity(KF.measurementMatrix);
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	setIdentity(KF.measurementNoiseCov, Scalar::all(10));
	setIdentity(KF.errorCovPost, Scalar::all(.1));

	//Point p(0,0);
	// Main loop
	while(true)
	{
		cam >> frame;
		ROI = frame(rect);
		/*
		cvtColor(frame,frame,CV_BGR2HSV);
		setMouseCallback("Main video",mouseHandler,(void*)(&p));
		
		if (p.x != 0 && p.y != 0)
		{
			showColorFromClick(frame,p);
			p.x = 0;
			p.y = 0;
		}
		*/

		playerDetected = false;
		detectPlayer();
		//player.printPlayer();

		if (playerDetected) 
		{
			checkBoundaries();
			handsDetected = false;
			detectHands();
			if (handsDetected)
			{
				takeAction();
			}
		}
		
		imshow("Main video",frame);
		imshow("ROI",ROI);
		key = waitKey(30);
	}

	return 0;
}

void detectPlayer()
{
	Mat frameHSV;
	Mat segmented;
	vector<double> playerAttributes = vector<double>(3); // [cX,cY,area]

	cvtColor(ROI,frameHSV,CV_BGR2HSV);
	segmented = segmentColor(frameHSV,colorHSV,toleranceHSV);
	
	dilate(segmented,segmented,STREL);
	erode(segmented,segmented,STREL);

	imshow("Segmented",segmented);
	playerAttributes = drawMoments(segmented,ROI,minArea);
	
	// First predict, to update the internal statePre variable
	Mat prediction = KF.predict();
	Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
             
	// Get mouse point
	measurement(0) = playerAttributes[0];
	measurement(1) = playerAttributes[1];
             
	//Point measPt(measurement(0),measurement(1));
 
	// The "correct" phase that is going to use the predicted value and our measurement
	Mat estimated = KF.correct(measurement);
	Point statePt(estimated.at<float>(0),estimated.at<float>(1));
	// circle(ROI,statePt,10, Scalar(255,0,0),-1);

	playerAttributes[0] = estimated.at<float>(0);
	playerAttributes[1] = estimated.at<float>(1);
	
	setPlayerAttributes(playerAttributes);
	
	//setPlayerAttributes(playerAttributes);
	frameHSV.release();
	segmented.release();
}

void setPlayerAttributes(vector<double> pAttrib)
{
	if (pAttrib[0] != 0.0 && pAttrib[1] != 0.0 && pAttrib[2] != 0.0) {
		player.setCX(pAttrib[0]);
		player.setCY(pAttrib[1]);
		player.setArea(pAttrib[2]);
		playerDetected = true;
	}
}

void detectHands()
{
	Mat frameHSV;
	Mat segmented;
	vector<double> handAttributes = vector<double>(6); // [cX,cY,area]

	cvtColor(ROI,frameHSV,CV_BGR2HSV);
	segmented = segmentColor(frameHSV,armColorHSV,armToleranceHSV);
			
	erode(segmented,segmented,STREL);
	dilate(segmented,segmented,STREL);

	imshow("Segmented",segmented);
	handAttributes = drawMoments(segmented,ROI,0, player);

	setHandAttributes(handAttributes);
	frameHSV.release();
	segmented.release();
}

void setHandAttributes(vector<double> hAttrib)
{
	if (hAttrib[0] != 0.0 && hAttrib[1] != 0.0 && hAttrib[2] != 0.0)
	{
		if (hAttrib[0] > player.getCX()) // 0 -> Left
		{
			player.setLeftHand(Hand(hAttrib[0],hAttrib[1],hAttrib[2]));
		} else { // 0 -> Right
			player.setRightHand(Hand(hAttrib[0],hAttrib[1],hAttrib[2]));
		}
		handsDetected = true;
	}

	if (hAttrib[3] != 0.0 && hAttrib[4] != 0.0 && hAttrib[5] != 0.0) 
	{
		if (hAttrib[3] > player.getCX()) // 3 -> Left
		{
			player.setLeftHand(Hand(hAttrib[3],hAttrib[4],hAttrib[5]));
		} else { // 3 -> Right
			player.setRightHand(Hand(hAttrib[3],hAttrib[4],hAttrib[5]));
		}
		handsDetected = true;
	}
}

void takeAction()
{
	if (player.getCY() - player.getLeftHand().getCY() > minHeight)
	{
		if (!(player.getCY() - player.getRightHand().getCY() > minHeight))
		{
			cout << "Left hand raised" << endl;
		}
	} else if (player.getCY() - player.getRightHand().getCY() > minHeight) 
	{
		cout << "Right hand raised" << endl;
	}
}

void checkBoundaries()
{
	int epsilon = 10;

	if (player.getCX() >= WIDTH - epsilon) 
	{
		cout << "Move camera to the right" << endl;
	} else if (player.getCX() <= epsilon)
	{
		cout << "Move camera to the left" << endl;
	}
}