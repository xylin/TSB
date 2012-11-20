#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Utilities.h"
#include "RegionLabelling.h"
#include "AntiGeometricDiffusion.h"

#include "PreProcessing.h"


void PreProcessing(const Mat& matInput, Mat&matBalckHat, Mat& matAntiGeo)
{
	Mat gray;

	cvtColor(matInput, gray, CV_BGR2GRAY);
	
	int iWidth = matInput.cols;
	int iHeight = matInput.rows;

	float *pfInput = new float[iWidth*iHeight];

	int iCount (0);

	for(int j=0; j<iHeight; j++)
	{
		for(int i=0; i<iWidth; i++)
		{
			pfInput[iCount] = gray.at<uchar>(j, i);

			iCount++;
		}
	}

	float *pfOutput = new float[iWidth*iHeight];
	memset(pfOutput, 0, sizeof(float)*iWidth*iHeight);

	float fDelta = 0.5;
	int iMaxIterations =10;
	int iType = 1;

	AntiGeometricDiffusionInSlice(pfInput, fDelta, iMaxIterations, iType, iHeight, iWidth, pfOutput);

	matAntiGeo.create(gray.size(), gray.type());
	matAntiGeo.setTo(0);

	iCount = 0;

	for(int j=0; j<iHeight; j++)
	{
		for(int i=0; i<iWidth; i++)
		{
			matAntiGeo.at<uchar>(j, i) = pfOutput[iCount];

			iCount++;
		}
	}

//	Mat dump;	

//	medianBlur(matAntiGeo, dump, 3);
	
//	matAntiGeo = dump;
	
	imwrite("C:/Development/projects/TSB/antiGeo.jpg", matAntiGeo);

	int element_shape = MORPH_RECT;

	int an =20;
	Mat element = getStructuringElement(element_shape, Size(an*2+1, an*2+1), Point(an, an) );
	
	matBalckHat.setTo(0);

	morphologyEx(matAntiGeo, matBalckHat, CV_MOP_BLACKHAT, element);
	
	imwrite("C:/development/projects/TSB/BackHat.jpg", matBalckHat);
}