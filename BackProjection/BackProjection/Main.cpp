#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/// Global Variables
Mat src; Mat hsv;
Mat mask;

int lo = 20; int up = 20;
const char* window_image = "Source image";

/// Function Headers
void Hist_and_Backproj( );
void pickPoint (int event, int x, int y, int, void* );

/**
 * @function main
 */
int main( int, char** argv )
{
	VideoCapture cam(0);

	while(true)
	{
		cam >> src;
		blur(src,src,Size(5,5));
		setMouseCallback( window_image, pickPoint, 0 );

		cvtColor(src,hsv,COLOR_BGR2HSV);

		imshow(window_image,src);
		waitKey(30);
	}

	return 0;
}

/**
 * @function pickPoint
 */
void pickPoint (int event, int x, int y, int, void* )
{
  if( event != EVENT_LBUTTONDOWN )
    { return; }

  // Fill and get the mask
  Point seed = Point( x, y );

  int newMaskVal = 255;
  Scalar newVal = Scalar( 120, 120, 120 );

  int connectivity = 8;
  int flags = connectivity + (newMaskVal << 8 ) + FLOODFILL_FIXED_RANGE + FLOODFILL_MASK_ONLY;

  Mat mask2 = Mat::zeros( src.rows + 2, src.cols + 2, CV_8UC1 );
  floodFill( src, mask2, seed, newVal, 0, Scalar( lo, lo, lo ), Scalar( up, up, up), flags );
  mask = mask2( Range( 1, mask2.rows - 1 ), Range( 1, mask2.cols - 1 ) );

  imshow( "Mask", mask );

  Hist_and_Backproj( );
}

/**
 * @function Hist_and_Backproj
 */
void Hist_and_Backproj( )
{
  MatND hist;
  int h_bins = 30; int s_bins = 32;
  int histSize[] = { h_bins, s_bins };

  float h_range[] = { 0, 179 };
  float s_range[] = { 0, 255 };
  const float* ranges[] = { h_range, s_range };

  int channels[] = { 0, 1 };

  /// Get the Histogram and normalize it
  calcHist( &hsv, 1, channels, mask, hist, 2, histSize, ranges, true, false );

  normalize( hist, hist, 0, 255, NORM_MINMAX, -1, Mat() );

  /// Get Backprojection
  MatND backproj;
  calcBackProject( &hsv, 1, channels, hist, backproj, ranges, 1, true );

  /// Draw the backproj
  imshow( "BackProj", backproj );

}