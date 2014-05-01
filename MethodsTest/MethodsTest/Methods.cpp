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

Mat segmentColor(Mat inputImage, Scalar color, Scalar tolerance) 
{
	Mat outputImage;
	
	Scalar zero(0,0,0);
	Scalar one(255,255,255);
	
	inRange(inputImage,max(zero,color-tolerance),min(one,color+tolerance),outputImage);
	
	return outputImage;
}

Scalar max(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = max(a[0],b[0]);
	out[1] = max(a[1],b[1]);
	out[2] = max(a[2],b[2]);
	return out;
}

Scalar min(Scalar a, Scalar b)
{
	Scalar out;
	out[0] = min(a[0],b[0]);
	out[1] = min(a[1],b[1]);
	out[2] = min(a[2],b[2]);
	return out;
}
