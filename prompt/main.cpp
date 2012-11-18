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

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
	Mat matOrig;
	matOrig = imread(argv[1]);

	if(matOrig.cols==0)
	{
		cerr << "Wrong input image" << endl;
		return 1;
	}

	
	Mat matPreprocessed;

	PreProcessing(matOrig, matPreprocessed);
	cerr << "Finished pre-processing" << endl;

	Mat matOutput;

	MeanShift(matPreprocessed, matOutput);

	cerr << "Finished mean shift" << endl;

	string strOutputFile = argv[2];

	imwrite(strOutputFile, matOutput);

	return 0;
}




