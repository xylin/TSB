#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;

void RemoveNoiseEdgePoint(const Mat&edgeIn, const int iSize, Mat&edgeOut);
void PostProcessing(Mat& matInput, Mat matMeanShift, Mat&matOutput);
void FillHole(const Mat& matIn, Mat& matOut);
void GetMS_FGD_Regions(Mat& imgMeanShiftResult, Mat&matMask, vector< vector<Point> >& vvptCrnRegion);
void GetOverlay(const Mat& maskCrn, const Mat& maskGlobal, Mat& maskOut);
#endif