#ifndef MY_LIBRARY_
#define MY_LIBRARY_

#include "opencv2/opencv.hpp"
#include <vector>

using namespace std;
using namespace cv;

Mat output, input, transformMatrix;
Point p1, p2, p3, p4;
Point2f src[4];
Point2f dst[4];
int cnt = 0;
int a = 1;

#endif
