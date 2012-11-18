#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#ifndef UTILITY_H
#define UTILITY_H

#define AUX_VALUE 100

#define QuickSortSwap(a,b) {double temp;temp=(a);(a)=(b);(b)=temp;}

using namespace std;
using namespace cv;

double log2( double n );

double EuclideanDist(const int& iX, const int& iY, const int& iXRef, const int& iYRef);

void FindEnclosing(const Mat& matDump, Rect& rctEncl);

void VectorQuickSortAscendingWithAux(vector<double>& A, int low, int high, vector<int>& auxiliary_list);

string SplitFilename(const string& str);

#endif