#include <cv.h>
#include "Methods.h"
#include <highgui.h>
using namespace cv;

Mat normalizeColors(Mat m)
{
	vector<Mat> channels;
	Mat output;
	Mat sum;

	split(m,channels);

	sum = channels[0] + channels[1] + channels[2];
	channels[0] = channels[0]/sum;
	channels[1] = channels[1]/sum;
	channels[2] = channels[2]/sum;
	
	merge(channels,output);

	sum.release();

	return output;
}

Mat segmentColor(Mat inputImage, int color[], int tolerance[]) 
{
	Mat outputImage;
	Mat Ired;
	Mat Igreen;
	Mat Iblue;
	vector<Mat> channels;
	
	split(inputImage,channels);
	inRange(channels[0],max(0,color[0]-tolerance[0]),min(255,color[0]+tolerance[0]),Iblue);
	inRange(channels[1],max(0,color[1]-tolerance[1]),min(255,color[1]+tolerance[1]),Igreen);
	inRange(channels[2],max(0,color[2]-tolerance[2]),min(255,color[2]+tolerance[2]),Ired);
	
	outputImage = Iblue & Igreen & Ired;

	Ired.release();
	Igreen.release();
	Iblue.release();

	return outputImage;
}