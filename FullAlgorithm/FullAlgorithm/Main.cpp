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
Scalar colorHSV(42,165,200); // Color detection parameters
Scalar toleranceHSV(6,90,55);
Scalar armColorHSV(0,0,0);
Scalar armToleranceHSV(0,0,0);
const int upperBound = 134;
const int lowerBound = 463;
Rect rect = Rect(WIDTH/4,upperBound,WIDTH/2,lowerBound-upperBound); // Bounding rectangle
const Mat STREL = getStructuringElement(MORPH_ELLIPSE, Size(7,7)); // Erode and dilate strel

// Global variables
Player player = Player(); // Player + Hands
bool playerDetected = false;
Mat frame;
Mat ROI;

KalmanFilter KF(4, 2, 0);
Mat_<float> measurement(2,1);

// Methods
void keyHandler(char k);
void detectPlayer();
void setPlayerAttributes(double cX, double cY, double area);
void detectHands();

int main()
{
	VideoCapture cam(0);
	cam.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT,HEIGHT);

	char key;

KF.transitionMatrix = *(Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
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
	while(true)
	{
		cam >> frame;
		ROI = frame(rect);
		//cvtColor(frame,frame,CV_BGR2HSV);
		//setMouseCallback("Main video",mouseHandler,(void*)(&p));
		/*
		iF (p.x != 0 && p.y != 0)
		{
			showColorFromClick(frame,p);
			p.x = 0;
			p.y = 0;
		}
		*/

		detectPlayer();
		player.printPlayer();
		if (playerDetected) 
		{
			detectHands();
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
	
	dilate(segmented,segmented,STREL);
	erode(segmented,segmented,STREL);

	imshow("Segmented",segmented);
	playerAttributes = drawMoments(segmented,ROI,minArea);
	/*
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
	circle(ROI,statePt,10, Scalar(255,0,0),-1);
	
	setPlayerAttributes(estimated.at<float>(0),estimated.at<float>(1),playerAttributes[2]);
	*/
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

	player.setLeftHand(Hand(handAttributes[0],handAttributes[1],handAttributes[2]));
	player.setRightHand(Hand(handAttributes[3],handAttributes[4],handAttributes[5]));

	/*
	// First predict, to update the internal statePre variable
	Mat prediction = KF.predict();
	Point predictPt(prediction.at<float>(0),prediction.at<float>(1));
             
	// Get mouse point
	measurement(0) = handAttributes[0];
	measurement(1) = handAttributes[1];
             
	//Point measPt(measurement(0),measurement(1));
 
	// The "correct" phase that is going to use the predicted value and our measurement
	Mat estimated = KF.correct(measurement);
	//Point statePt(estimated.at<float>(0),estimated.at<float>(1));
	//circle(original,statePt,10, Scalar(255,0,0),-1);

	prediction = KF.predict();
	predictPt = Point(prediction.at<float>(0),prediction.at<float>(1));

	measurement(0) = handAttributes[3];
	measurement(1) = handAttributes[4];

	estimated = KF.correct(measurement);
	//setPlayerAttributes(estimated.at<float>(0),estimated.at<float>(1),playerAttributes[2]);
	*/
	frameHSV.release();
	segmented.release();
}