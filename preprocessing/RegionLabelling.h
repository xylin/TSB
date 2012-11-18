#ifndef REGIONLABELLING_H
#define REGIONLABELLING_H

#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

typedef struct
{
	int i;
	int j;
}_PIXEL;

typedef stack <_PIXEL> STACK_PIXEL;

void RegionLabelling2D(const Mat& maskBGD, vector< vector<cv::Point> >& vvpRegions);

#endif