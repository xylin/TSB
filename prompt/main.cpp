////////////////////////////////////////////////////////
// Name     : main.cpp
// Purpose  : Edge Detection and Image SegmentatiON
//            (EDISON) Command Prompt System
// Author   : Chris M. Christoudias
// Modified by
// Created  : 03/20/2002
// Copyright: (c) Chris M. Christoudias
// Version  : v0.1
////////////////////////////////////////////////////////

//include local and system libraries and definitions
#include "defs.h"
#include "error.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../preprocessing/MSWrap.h"
#include "../preprocessing/PreProcessing.h"
#include "../preprocessing/PostProcessing.h"

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat matOrig;
	matOrig = imread(argv[1]);

	/*
	Rect rctcrop;

	rctcrop.x = 1000;
	rctcrop.y = 1000;
	rctcrop.width = 512;
	rctcrop.height = 512;

	Mat matcrop;

	matcrop = matOrig(rctcrop);

	imwrite("C:/development/projects/TSB/10_h_crop.jpg", matcrop);

	return 0;


	if(matOrig.cols==0)
	{
		cerr << "Wrong input image" << endl;
		return 1;
	}
	*/
	
	Mat matBlackHat;
	Mat matAntiGeo;

	PreProcessing(matOrig, matBlackHat, matAntiGeo);
	cerr << "Finished pre-processing" << endl;

	Mat matMeanShift;

	MeanShift(matBlackHat, matMeanShift);

	cerr << "Finished mean shift" << endl;

	imwrite("C:/development/projects/TSB/MeanShift.jpg", matMeanShift);

	Mat matOutput;	

/*	PostProcessing(matBlackHat, matMeanShift, matOutput);


	string strOutputFile = argv[2];

	imwrite(strOutputFile, matOutput);	*/

	return 0;
}




