/*
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include "Methods.h"

using namespace std;
using namespace cv;

// Global variables
Mat frame;

// Methods

int main()
{
	VideoCapture cam(0);

	Point p(0,0);
	while(true)
	{
		cam >> frame;
		
		setMouseCallback("Main video",mouseHandler,(void*)(&p));
		
		if (p.x != 0 && p.y != 0)
		{
			cout << "(" << p.x << "," << p.y << ")" << endl;
			p.x = 0;
			p.y = 0;
		}
		imshow("Main video",frame);
		waitKey(30);
	}
	return 0;
}

*/