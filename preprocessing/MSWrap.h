#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#ifndef MSWRAP_H
#define MSWRAP_H


#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "../prompt/defs.h"
#include "../prompt/error.h"
#include "../prompt/parser.h"


using namespace cv;

void CreateEDS(const int SpatialBandwidth, const int RangeBandwidth, const int MinimumRegionArea, const double EdgeStrengthThreshold);

int MAIN_EDISON(int argc, char **argv);

void MeanShift(const Mat&imgOrig, Mat&MS_Result);


//check syntax of source file
CmCError CheckSyntax(char *filename, CmCParser *synParser);

//run the script
CmCError Run(CmCParser *script);

//report system errors
void Report(CmCError *error, char *srcFilename, CmCParser *srcParser);

#endif