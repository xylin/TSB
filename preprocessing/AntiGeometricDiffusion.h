#ifndef ANTIGEOMETRICDIFFUSION_H
#define ANTIGEOMETRICDIFFUSION_H


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"



void AntiGeometricDiffusionInSlice(const float* pfInput, float fDelta, int iMaxIterations, int iType, int iHeight, int iWidth, float*pfOutput);

#endif