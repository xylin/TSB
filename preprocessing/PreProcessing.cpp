#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Utilities.h"
#include "RegionLabelling.h"
#include "AntiGeometricDiffusion.h"

#include "PreProcessing.h"


void PreProcessing(const Mat& matInput, Mat&matOutput)
{
	Mat dst1;

	int element_shape = MORPH_RECT;

	int an =20;
	Mat element = getStructuringElement(element_shape, Size(an*2+1, an*2+1), Point(an, an) );

	morphologyEx(matInput, dst1, CV_MOP_BLACKHAT, element);

	Mat gray;

	cvtColor(dst1, gray, CV_BGR2GRAY);

	Mat edge;

	blur(gray, edge, Size(5,5));

	Canny(edge, edge, 10, 15, 3);

	imwrite("C:/development/projects/TSB/BackPHAP.jpg", dst1);
	imwrite("C:/development/projects/TSB/BackPHAP_edge.jpg", edge);

	int iWidth = edge.cols;
	int iHeight = edge.rows;

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

	float fDelta =1.5;
	int iMaxIterations = 1;
	int iType = 0;

	AntiGeometricDiffusionInSlice(pfInput, fDelta, iMaxIterations, iType, iHeight, iWidth, pfOutput);

	matOutput.create(gray.size(), gray.type());
	matOutput.setTo(0);

	iCount = 0;

	for(int j=0; j<iHeight; j++)
	{
		for(int i=0; i<iWidth; i++)
		{
			matOutput.at<uchar>(j, i) = pfOutput[iCount];

			iCount++;
		}
	}

	Mat dump;
	dump;

	medianBlur(matOutput, dump, 3);

	matOutput = dst1;

	imwrite("C:/Development/projects/TSB/antiGeo.jpg", matOutput);
}