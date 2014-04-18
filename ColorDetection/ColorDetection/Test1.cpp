#include <iostream>
#include <cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

const int ARRAY_SIZE = 6; // Array for frame average
const double AREA_THRESH = 50.0; // Minimum valid area for theory of moments

vector<double> xCentroid;
vector<double> yCentroid;

Mat segmentColor(Mat , int[] , int[] );
void showValue(Mat , Point );
void drawMoments(Mat, Mat &);
Mat normalizeImage(Mat );
void mouseHandler(int , int , int , int , void* );
int maxAreaIndex(vector<double> );
vector<double> getCentroid(double , double );
double average(vector<double> );
void setColorFromClick(Mat ,int* ,Point );
void changeTolerance(char , int* );

int main() 
{
	Mat frame;
	Size kernel = Size(25,25);
	Mat colorFrame;
	Mat Iblurred;
	Mat Isegmented;
	Mat HSVframe;
	Mat strel = getStructuringElement(MORPH_ELLIPSE, Size(7,7));
	char k;

	int color[] = {127,242,195};
	int tolerance[] = {15,15,15};
	int colorHSV[] = {124*(180)/240,139*(256)/240,177*(256)/240};      //  Note: For HSV, Hue range is [0,179], Saturation range is [0,255] 
	int toleranceHSV[] = {20,15,20};   //  and Value range is [0,255]. Paint 240.

	VideoCapture cam(0);
	
	Point p(0,0);
	while (true)
	{
		cam >> colorFrame;
		//colorFrame = normalizeImage(colorFrame);

		setMouseCallback("Main camera",mouseHandler,(void*)(&p));
		
		if (p.x != 0 && p.y != 0) {
			//showValue(colorFrame,p);
			setColorFromClick(colorFrame,color,p);
		}
		
		// BGR approach
		GaussianBlur(colorFrame, Iblurred, kernel,5);
		//Iblurred = normalizeImage(Iblurred);
		//imshow("Smoothed camera",Iblurred);
		
		Isegmented = segmentColor(Iblurred,color,tolerance);
		dilate(Isegmented,Isegmented,strel);
		erode(Isegmented,Isegmented,strel);
		imshow("Segmented video",Isegmented);

		drawMoments(Isegmented,colorFrame);
		imshow("Main camera",colorFrame);
		/*
		// HSV approach
		cvtColor(colorFrame,HSVframe,CV_BGR2HSV);
		GaussianBlur(HSVframe, HSVframe, kernel,5);
		Isegmented = segmentColor(HSVframe,colorHSV,toleranceHSV);
		dilate(Isegmented,Isegmented,strel);
		erode(Isegmented,Isegmented,strel);
		imshow("Segmented video HSV",Isegmented);

		//drawMoments(Isegmented,HSVframe);
		imshow("Main HSV camera",HSVframe);
		*/
		k = waitKey(30);
		changeTolerance(k,tolerance);
	}

	destroyWindow("Main camera");
	return 0;
}

/*  Recognizes some color, given a base value and a tolerance.
	It is independent from the color space (BGR,HSV,etc).
	Inputs:  -inputImage, the image to be segmented.
			 -color[], a three dimensional vector, which represents the color.
			 -tolerance[], a three dimensional vector, which represents the maximum
			  variations allowed from the color.
	Outputs: -outputImage, a segmented image.
*/
Mat segmentColor(Mat inputImage, int color[], int tolerance[]) 
{
	Mat outputImage;
	Mat Ired;
	Mat Igreen;
	Mat Iblue;
	vector<Mat> channels;
	
	split(inputImage,channels);
	Iblue = (channels[0] > color[0] - tolerance[0]) & (channels[0] < color[0] + tolerance[0]);
	Igreen = (channels[1] > color[1] - tolerance[1]) & (channels[1] < color[1] + tolerance[1]);
	Ired = (channels[2] > color[2] - tolerance[2]) & (channels[2] < color[2] + tolerance[2]);

	outputImage = Iblue & Igreen & Ired;

	Ired.release();
	Igreen.release();
	Iblue.release();

	return outputImage;
}

Mat normalizeImage(Mat image) 
{
	Mat sum;
	Mat outputImage;
	vector<Mat> channels;
	
	split(image,channels);

	sum = channels[0] + channels[1] + channels[2] + 0.00001; // To avoid zero division error.
	channels[0] = (channels[0]/sum)*255;
	channels[1] = (channels[1]/sum)*255;
	channels[2] = (channels[2]/sum)*255;
	
	merge(channels,outputImage);

	sum.release();

	return outputImage;
}

/*  Shows the value of a single point in the current frame.
	The frames can be expressed in several color spaces (BGR,HSV,etc)
*/
void showValue(Mat image,Point p)
{
	vector<Mat> channels;

	split(image,channels);
	cout << p << " = [" << (int)channels[0].at<uchar>(p) << "," << \
		(int)channels[1].at<uchar>(p) << "," << \
		(int)channels[2].at<uchar>(p) << "]" << endl;
}

/*  Draws the moments for a given frame.
	Inputs: -imageBW, a segmented image, from which the objects will be extracted.
			-original, the image in which the moments will be drawn.
	Outputs: None
*/
void drawMoments(Mat imageBW, Mat &original) 
{
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	RNG rng(12345);

	findContours(imageBW, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
	vector<Moments> mu; 

	for( int i = 0; i < contours.size(); i++ )
	{ 
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
	
	/*
	for (int i = 0; i < contours.size(); i++)
	{
		if (area[i] > AREA_THRESH) {
			cout << area[i] << endl;
			circle(original,Point(cX[i],cY[i]),20, Scalar(0,0,255),-1);
		}
	}
	*/
	
	int maxIndex = maxAreaIndex(area);

	
	vector<double> centroid;
	centroid.push_back(0);
	centroid.push_back(0);

	if (maxIndex != -1 && area[maxIndex] >= AREA_THRESH) 
	/*{
		cout << area[maxIndex] << endl;
		centroid = getCentroid(cX[maxIndex],cY[maxIndex]);
		circle(original,Point(centroid[0],centroid[1]),20, Scalar(0,0,255),-1);
	}
	*/
	{
		cout << area[maxIndex] << endl;
		circle(original,Point(cX[maxIndex],cY[maxIndex]),20, Scalar(0,0,255),-1);
	}
	
	cout << "New loop" << endl;
}


//TO-DO : Implement the function to use an average of the first ARRAY_SIZE frames.
Mat getFrame(vector<Mat> &frames, Mat frame) 
{
	static Mat sum;
	//Mat sum = Mat::zeros(rows,cols,CV
	for (int i = 0; i < ARRAY_SIZE; i++) 
	{
		//sum += frames[i];
	}
	//sum += frame;

	frames.push_back(frame);
	//return sum/SIZE+1;
	Mat out;
	return out;
}

void mouseHandler(int event, int x, int y, int flags, void* param)
{
	switch(event){
    case CV_EVENT_LBUTTONUP:
        cout << "x = " << x << "\ny = " << y << endl;
		cv::Point* p = (cv::Point*)param;
		p->x = x;
		p->y = y;
        break;
    }
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

// TO-DO: Fix (Not working).
vector<double> getCentroid(double newX, double newY)
{
	static vector<double> centroidsX;
	static vector<double> centroidsY;
	vector<double> centroids;
	centroids.push_back(0);
	centroids.push_back(0);
	vector<double>::iterator it;

	if (centroidsX.size() < 6) 
	{
		centroidsX.push_back(newX);
		centroidsY.push_back(newY);
		centroids[0] = newX;
		centroids[1] = newY;
	} else {
		centroidsX.pop_back();
		centroidsY.pop_back();

		it = centroidsX.begin();
		centroidsX.insert(it,newX);
		it = centroidsY.begin();
		centroidsX.insert(it,newY);

		centroids[0] = average(centroidsX);
		centroids[1] = average(centroidsY);
	}

	String cX = "[";
	String cY = "[";
	for(int i=0; i < centroids.size(); i++)
	{
		cX += char(centroidsX[i]) + ",";
		cY += char(centroidsY[i]) + ",";
	}
	cout << cX << "\n" << cY;
	return centroids;
}

double average(vector<double> v) 
{
	double sum = 0;
	for (int i = 0; i < v.size(); i++) 
	{
		sum += v[i];
	}
	
	if (v.size() != 0)
	{
		sum /= v.size();
	}

	return sum;
}

void setColorFromClick(Mat frame,int* color,Point p)
{
	vector<Mat> channels;

	split(frame,channels);
	cout << "Set color\n";
	color[0] = (int)channels[0].at<uchar>(p);
	color[1] = (int)channels[1].at<uchar>(p);
	color[2] = (int)channels[0].at<uchar>(p);

	cout << "[" << color[0] << "," << color[1] << "," << color[2] << "]\n";

}

void changeTolerance(char k, int* tolerance)
{
	if (k == '+')
	{
		tolerance[0] += 1;
		tolerance[1] += 1;
		tolerance[2] += 1;
	} else if (k == '-')
	{
		tolerance[0] -= 1;
		tolerance[1] -= 1;
		tolerance[2] -= 1;
	}
}