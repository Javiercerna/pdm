#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/video/background_segm.hpp>

using namespace std;
using namespace cv;

// Constants
const int BLUR_SIZE = 5;
const Mat strel = getStructuringElement(MORPH_ELLIPSE, Size(7,7));

// Global variables
Mat colorFrame;
Mat foreground;
Ptr<BackgroundSubtractor> pMOG;
int tshirtColor[] = {127,242,195};
int tshirtTolerance[] = {50,50,50};
int handColor[] = {1,2,3};
int handTolerance[] = {50,50,50};
double tshirt[] = {0,0,0}; // [0] = cX, [1] = cY, [2] = area
double leftHand[] = {0,0,0};
double rightHand[] = {0,0,0};

// Methods
void trackPlayer();
void getForeground();
void processForeground();
Mat segmentColor(Mat inputImage, int color[], int tolerance[]);
void drawMoments(Mat imageBW, Mat &original, double* object);
int maxAreaIndex(vector<double> areas);
void showObjectProperties(double* object);
void mouseHandler(int event, int x, int y, int flags, void* param);
void setColorFromClick(Mat frame, int* color, Point p);
void keyPressed(char k);

int main()
{
	VideoCapture cam(0);
	pMOG = new BackgroundSubtractorMOG(3, 3, 0.4,1.0);

	char key;
	Point mousePoint(0,0);

	while (true)
	{
		cam >> colorFrame;

		setMouseCallback("Main camera",mouseHandler,(void*)(&mousePoint));

		if (mousePoint.x != 0 && mousePoint.y != 0) {
			setColorFromClick(colorFrame,tshirtColor,mousePoint);
			mousePoint.x = 0;
			mousePoint.y = 0;
		}

		trackPlayer();

		imshow("Main camera",colorFrame);

		key = waitKey(30);
		keyPressed(key);
	}
	return 0;
}

void trackPlayer() 
{
	getForeground();

	processForeground();
	
}

void getForeground()
{
	Mat motionMask;
	
	blur(colorFrame,colorFrame,Point(BLUR_SIZE,BLUR_SIZE));
	pMOG->operator()(colorFrame, motionMask);

	foreground = Mat::zeros(foreground.rows,foreground.cols,foreground.type());

	colorFrame.copyTo(foreground,motionMask);
	imshow("FG Mask MOG", foreground);

	motionMask.release();
}

void processForeground()
{
	Mat tshirtMask;
	Mat handsMask;

	// Tshirt
	tshirtMask = segmentColor(foreground,tshirtColor,tshirtTolerance);
	imshow("Masked segmentation",tshirtMask);

	dilate(tshirtMask,tshirtMask,strel);
	erode(tshirtMask,tshirtMask,strel);

	drawMoments(tshirtMask,colorFrame,tshirt);
	cout << "Tshirt properties:" << endl;
	showObjectProperties(tshirt);

	// Hands
	/*
	handsMask = segmentColor(foreground,handColor,handTolerance);
	imshow("Masked hands",handsMask);

	//dilate(handsMask,handsMask,strel);
	//erode(handsMask,handsMask,strel);

	drawMoments(handsMask,colorFrame,leftHand,rightHand);
	cout << "Left hand properties:" << endl;
	showObjectProperties(leftHand);
	cout << "Right hand properties:" << endl;
	showObjectProperties(rightHand);
	*/

	tshirtMask.release();
	handsMask.release();
}

Mat segmentColor(Mat inputImage, int color[], int tolerance[]) 
{
	Mat outputImage;
	Mat Ired;
	Mat Igreen;
	Mat Iblue;
	vector<Mat> channels;
	
	split(inputImage,channels);

	Iblue = (channels[0] > color[0] - tolerance[0]) & (channels[0] < color[0] + tolerance[0]) \
		& (channels[0] > 0);
	Igreen = (channels[1] > color[1] - tolerance[1]) & (channels[1] < color[1] + tolerance[1]) \
		& (channels[1] > 0);
	Ired = (channels[2] > color[2] - tolerance[2]) & (channels[2] < color[2] + tolerance[2]) \
		& (channels[2] > 0);

	outputImage = Iblue & Igreen & Ired;

	Ired.release();
	Igreen.release();
	Iblue.release();

	return outputImage;
}

/*
	imageBW: Segmented image.
	original: Frame.
	object: tshirt or hand.
*/
// TO-DO Handle the case for two hands ( drawMoments(foreground, colorFrame, leftHand, rightHand) )
void drawMoments(Mat imageBW, Mat &original, double* object) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 
	vector<Rect> boundRect(contours.size());

	for( int i = 0; i < contours.size(); i++ )
	{ 
		boundRect[i] = boundingRect(Mat(contours[i]));
		mu.push_back(moments(contours[i], false)); 
	}

	//Centroids 
	vector<double> cX;
	vector<double> cY;
	vector<double> area;

	for (int i = 0; i < contours.size(); i++)
	{
		cX.push_back(mu[i].m10/mu[i].m00);
		cY.push_back(mu[i].m01/mu[i].m00);
		area.push_back(mu[i].m00);
	}

	int maxIndex = maxAreaIndex(area);

	if (maxIndex != -1) 
	{
		cout << area[maxIndex] << endl;

		object[0] = cX[maxIndex];
		object[1] = cY[maxIndex];
		object[2] = area[maxIndex];

		circle(original,Point(cX[maxIndex],cY[maxIndex]),20, Scalar(0,0,255),-1);
		rectangle(original, boundRect[maxIndex].tl(), boundRect[maxIndex].br(), Scalar(255,0,0), 2, 8, 0 );
	}
	
	cout << "New loop" << endl;
}

int maxAreaIndex(vector<double> areas)
{
	double maxArea = 0.0;
	int maxIndex = -1;

	for (int i = 0; i < areas.size(); i++)
	{
		if (areas[i] > maxArea)
		{
			maxArea = areas[i];
			maxIndex = i;
		}
	}

	return maxIndex;
}

void showObjectProperties(double* object)
{
	if (!(object[0] == 0.0 && object[1] == 0.0 && object[2] == 0.0))
	{
		cout << "cX = " << object[0] << endl \
			 << "cY = " << object[1] << endl \
			 << "area = " << object[2] << endl;
	}
}
void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
    case CV_EVENT_LBUTTONUP:
		cv::Point* p = (cv::Point*)param;
		p->x = x;
		p->y = y;
        break;
    }
}

void setColorFromClick(Mat frame,int* color,Point p)
{
	vector<Mat> channels;

	split(frame,channels);
	color[0] = (int)channels[0].at<uchar>(p);
	color[1] = (int)channels[1].at<uchar>(p);
	color[2] = (int)channels[0].at<uchar>(p);
	cout << "[" << color[0] << "," << color[1] << "," << color[2] << "]" << endl;
}

void keyPressed(char k)
{
	switch (k)
	{
	case '+':
		tshirtTolerance[0]++;
		tshirtTolerance[1]++;
		tshirtTolerance[2]++;
		cout << "Tolerance: [" << tshirtTolerance[0] << "," << tshirtTolerance[1] \
			<< "," << tshirtTolerance[2] << "]" << endl;
		break;
	case '-':
		if (tshirtTolerance[0] > 0)
		{
			tshirtTolerance[0]--;
			tshirtTolerance[1]--;
			tshirtTolerance[2]--;
			cout << "Tolerance: [" << tshirtTolerance[0] << "," << tshirtTolerance[1] \
			<< "," << tshirtTolerance[2] << "]" << endl;
		}
		break;
	}
}