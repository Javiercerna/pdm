#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

// Global variables
Mat frame;
Rect rect = Rect(240,210,200,480-210); // Bounding rectangle

int main()
{
	VideoCapture cam(0);
	Mat bounded;

	while(true)
	{
		cam >> frame;
		bounded = frame(rect);
		imshow("Main camera",frame);
		imshow("Bounded",bounded);
		waitKey(30);
	}
	return 0;
}