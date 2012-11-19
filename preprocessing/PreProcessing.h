#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;


void PreProcessing(const Mat& matInput, Mat&matOutput, Mat&matMask);

#endif