#include <cv.h>
#include "Methods.h"
#include <highgui.h>
using namespace cv;

Mat normalizeColors(Mat m)
{
	vector<Mat> channels;
	Mat output;
	Mat c1;
	Mat c2;
	Mat c3;

	split(m,channels);

	c1 = channels[0]/(channels[0]+channels[1]+channels[2]);
	c2 = channels[1]/(channels[0]+channels[1]+channels[2]);
	c3 = channels[2]/(channels[0]+channels[1]+channels[2]);
	
	channels[0] = c1;
	channels[1] = c2;
	channels[2] = c3;
	
	merge(channels,output);

	c1.release();
	c2.release();
	c3.release();

	return output;
}