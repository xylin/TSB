#include "MSWrap.h"

using namespace std;
using namespace cv;

void MeanShift(const Mat&imgOrig, Mat&MS_Result)
{	
	const int SpatialBandwidth = 7;
	const int RangeBandwidth = 6;
	const int MinimumRegionArea = 500;
	const double EdgeStrengthThreshold = 0.0001;

	CreateEDS(SpatialBandwidth, RangeBandwidth, MinimumRegionArea, EdgeStrengthThreshold);
	
	imwrite("InputEDISON.ppm", imgOrig);
	//imwrite(strPPMInput.c_str(), imgOrig);
		
	int argc=2;

	string str1="dump";
	string str2="EDISON.eds";
	
	char ** argv = new char*[2];
	char * pCH1 = const_cast<char *> (str1.c_str());
	char * pCH2 = const_cast<char *> (str2.c_str());
	
	argv[0]=pCH1;
	argv[1]=pCH2;	

	MAIN_EDISON(argc, argv);		
		
	MS_Result = imread("EDISON.ppm");	
}

void CreateEDS(const int SpatialBandwidth, const int RangeBandwidth, const int MinimumRegionArea, const double EdgeStrengthThreshold)
{
	string strEDS = "EDISON.eds";	

	ofstream outIndiEds(strEDS.c_str());

	outIndiEds << "SpatialBandwidth = " << SpatialBandwidth <<";" << endl;
	outIndiEds << "RangeBandwidth = " << RangeBandwidth <<";" << endl;
	outIndiEds << "MinimumRegionArea = " << MinimumRegionArea << ";" << endl;
	outIndiEds << "Speedup = NONE;" << endl;
	outIndiEds << "GradientWindowRadius = 2;" << endl;
	outIndiEds << "MixtureParameter = 0.1;" << endl;
	outIndiEds << "EdgeStrengthThreshold = " << EdgeStrengthThreshold << ";" << endl;

	outIndiEds << "Load('" << "InputEDISON.ppm" << "', IMAGE);" << endl;
	//outIndiEds << "Load('" << strPPMInput << "', IMAGE);" << endl;

	outIndiEds << "Synergistic ON;" << endl;

	outIndiEds << "UseCustomWeightMap OFF;" << endl;

	outIndiEds << "DisplayProgress ON;" << endl;

	//Segment the image
	outIndiEds << "Segment;"<< endl;

	//Save the resulting segmented image with the region boundaries
	//overlayed.
	outIndiEds << "Save('" << "EDISON.ppm" << "', PPM, SEGM_IMAGE_BOUNDARIES);" << endl;
	//outIndiEds << "Save('" << strPPMOutput << "', PPM, SEGM_IMAGE);" << endl;

	outIndiEds.close();		
}

int MAIN_EDISON(int argc, char **argv)
{
  //check usage
  if(argc != 2) {
      fprintf(stderr, "--------------------------------------------------\n");
      fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
      fprintf(stderr, "--------------------------------------------------\n");
      fprintf(stderr, "filename: File containing EDISON execution script.\n");
      fprintf(stderr, "--------------------------------------------------\n");
      exit(1);
  }
 // fprintf(stderr, "OK\n");
  //parse the source file and check for
  //syntaxical errors
  CmCParser *srcParser = new CmCParser;
  CmCError error = CheckSyntax(argv[1], srcParser);
  if(error.errorCode_) {
    Report(&error, argv[1], srcParser);
	fprintf(stderr, "Here\n");

    exit(1);
  }
//fprintf(stderr,"Finsihed CheckSyn\n");
  //execute script
  error = Run(srcParser);
  if(error.errorCode_) {
    Report(&error, argv[1], srcParser);
	fprintf(stderr, "there\n");
    exit(1);
  }

//fprintf(stderr,"Finsihed run\n");
  //de-allocate parser
  delete srcParser;

  return 0;
}

