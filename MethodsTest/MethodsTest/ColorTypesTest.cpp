/*
#include <cv.h>
#include <highgui.h>
#include <iostream>

using namespace std;
using namespace cv;

// Constants
const int WIDTH = 480;
const int HEIGHT = 640;

int main()
{
	Mat mUchar = Mat(WIDTH,HEIGHT,CV_16UC3,Scalar(50.0,100.0,255.0));
	Mat mFloat = Mat(WIDTH,HEIGHT,CV_32FC3,Scalar(1.0/2,1.0/4,1.0/8));
	
	while(true)
	{
		imshow("Scalar matrix image",mUchar);
		imshow("Float matrix image",mFloat);
		waitKey(30);
	}

	return 0;
}

*/