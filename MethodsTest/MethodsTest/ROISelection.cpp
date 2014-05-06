#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

// Constants
const int WIDTH = 640;
const int HEIGHT = 480;
const int upperBound = 134;
const int lowerBound = 463;
Rect rect = Rect(WIDTH/4,upperBound,WIDTH/2,lowerBound-upperBound); // Bounding rectangle

// Global variables
Mat frame;

int main()
{
	VideoCapture cam(0);
	Mat bounded;

	while(true)
	{
		cam >> frame;
		bounded = frame(rect);
		circle(bounded,Point(50,50),5,Scalar(255,0,0),-1);
		imshow("Main camera",frame);
		imshow("Bounded",bounded);
		waitKey(30);
	}
	return 0;
}