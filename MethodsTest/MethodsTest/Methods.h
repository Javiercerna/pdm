#include <cv.h>

using namespace cv;

Mat normalizeColors(Mat m);
Mat segmentColor(Mat inputImage, Scalar color, Scalar tolerance);
Scalar max(Scalar a, Scalar b);
Scalar min(Scalar a, Scalar b);