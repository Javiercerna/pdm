#include <cv.h>
#include "Player.h"

using namespace cv;

Mat normalizeColors(Mat m);
Mat segmentColor(Mat inputImage, Scalar color, Scalar tolerance);
Scalar max(Scalar a, Scalar b);
Scalar min(Scalar a, Scalar b);
vector<double> drawMoments(Mat imageBW, Mat &original,double minArea);
vector<double> drawMoments(Mat imageBW, Mat &original,double minArea, Player p);
int maxAreaIndex(vector<double> areas);
void mouseHandler(int event, int x, int y, int flags, void* param);
void showColorFromClick(Mat m,Point p);
double getDistance(Point p1, Point p2);