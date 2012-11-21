#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "Utilities.h"
#include "RegionLabelling.h"
#include "AntiGeometricDiffusion.h"
#include "PostProcessing.h"


void PostProcessing(Mat& matAntiGeo, Mat matMeanShift, Mat&matOutput)
{
	Mat gray;

	cvtColor(matAntiGeo, gray, CV_BGR2GRAY);

	Mat edge;

	blur(gray, edge, Size(5, 5));

	Canny(edge, edge, 5, 20, 3);

	Mat matEdgeFiltered;
	matEdgeFiltered.create(edge.size(), edge.type());
	matEdgeFiltered.setTo(0);

	const int iSize = 10;

	RemoveNoiseEdgePoint(edge, iSize, matEdgeFiltered);

//	imwrite("C:/development/projects/TSB/BackHat_edge.jpg", matEdgeFiltered);
		
//	int anA = 3;
//	int element_shapeA = 0;

//	Mat elementA = getStructuringElement(MORPH_RECT, Size(anA*2+1, anA*2+1), Point(anA, anA) );

//	Mat matMainObjOut;
//	morphologyEx( matEdgeFiltered, matMainObjOut, MORPH_CLOSE, elementA);			
	
//	Mat matFilledHole;
//	FillHole(matMainObjOut, matFilledHole);
//	imwrite("C:/development/projects/TSB/BackHat_edge_FilledHole.jpg", matFilledHole);

	Mat maskBGD_2PixelWider(matAntiGeo.rows+2, matAntiGeo.cols+2, CV_8UC1, Scalar::all(0));	
	
	vector < vector <Point> > vvptMS_FGD_Region;

	GetMS_FGD_Regions(matMeanShift, maskBGD_2PixelWider, vvptMS_FGD_Region);

	vector<bool> vbKeep(vvptMS_FGD_Region.size());

	for(int r=0; r<vvptMS_FGD_Region.size(); r++)
	{
		vector <Point>& vpReg = vvptMS_FGD_Region.at(r);

		for(int p=0; p<vpReg.size(); p++)
		{
			Point& pnt = vpReg.at(p);

			if(matEdgeFiltered.at<uchar>(pnt.y, pnt.x)>0)
			{
				vbKeep.at(r) = true;
				break;
			}
		}
	}
	
	matOutput.create(matEdgeFiltered.size(), matEdgeFiltered.type());
	matOutput.setTo(0);

	for(int r=0; r<vvptMS_FGD_Region.size(); r++)
	{
		if(vbKeep.at(r))
		{
			vector <Point>& vpReg = vvptMS_FGD_Region.at(r);
			for(int p=0; p<vpReg.size(); p++)
			{
				Point& pnt = vpReg.at(p);
				matOutput.at<uchar>(pnt.y, pnt.x) = 255;				
			}
		}
	}
}


void GetMS_FGD_Regions(Mat& imgMeanShiftResult, Mat&matMask_2PixelWider, vector< vector<Point> >& vvptCrnRegion)
{
	RNG rng = theRNG();    

	for(int y=0; y < imgMeanShiftResult.rows; y++ )
    {
        for(int x=0; x < imgMeanShiftResult.cols; x++ )
        {
			if( matMask_2PixelWider.at<uchar>(y+1, x+1) == 0 )
            {
                Scalar newVal( rng(256), rng(256), rng(256) );

				Mat maskDump;
				matMask_2PixelWider.copyTo(maskDump);

                floodFill(imgMeanShiftResult, matMask_2PixelWider, Point(x,y), newVal, 0, Scalar::all(0), Scalar::all(0), 8|CV_FLOODFILL_MASK_ONLY|255<<8);
				
				Mat currentMask(maskDump.rows, maskDump.cols, CV_8UC1, Scalar::all(0));

				GetOverlay(maskDump, matMask_2PixelWider, currentMask);
				
				vector< Point > vpnt;
				for(int y=1; y < currentMask.rows-1; y++ )
				{
					for(int x=1; x < currentMask.cols-1; x++ )
					{
						if( currentMask.at<uchar>(y, x) > 0 )
						{
							vpnt.push_back(Point(x-1,y-1));
						}
					}
				}

				vvptCrnRegion.push_back(vpnt);
			}				
        }		
    }	
}


void GetOverlay(const Mat& maskCrn, const Mat& maskGlobal, Mat& maskOut)
{
    int iRows = maskCrn.rows;
	int iCols = maskCrn.cols;
    
	const uchar*pcCrn=NULL;
	const uchar*pcGlobal=NULL;
    uchar*pcOut=NULL;
    
	for(int y=0; y < iRows; y++ )
    {
        pcCrn = maskCrn.ptr<uchar>(y);
        pcGlobal = maskGlobal.ptr<uchar>(y);
        pcOut = maskOut.ptr<uchar>(y);

        for(int x=0; x < iCols; x++ )
        {
			/*if( (maskCrn.at<uchar>(y, x) == 0) &&
				maskGlobal.at<uchar>(y, x) != 0 )
            {
				maskOut.at<uchar>(y,x) = 255; 				
			}*/
            if(*pcCrn==0 && *pcGlobal!=0)
                *pcOut = 255;
            
            pcOut++;
            pcCrn++;
            pcGlobal++;
            
		}
	}		
}
void RemoveNoiseEdgePoint(const Mat&edgeIn, const int iSize, Mat&edgeOut)
{
	vector< vector<Point> > vvpRegions;
    
	RegionLabelling2D(edgeIn, vvpRegions);
    
	for(int r=0; r<vvpRegions.size(); r++ )
	{
		vector<Point> & vpCrn=vvpRegions.at(r);
        
		if(vpCrn.size()>iSize)
		{
			for(int p=0; p<vpCrn.size(); p++)
			{
				Point& ptCrn = vpCrn.at(p);
                
				edgeOut.at<uchar>(ptCrn.y, ptCrn.x) = 255;
			}
		}
	}
}


void FillHole(const Mat& matIn, Mat& matOut)
{		
	int iHeight= matIn.rows;
	int iWidth = matIn.cols;

	int iFrameSize = iWidth * iHeight;
	int iWidthM1 = iWidth - 1;
	int iHeightM1 = iHeight - 1;

	matIn.copyTo(matOut);

	for(int j=0;j< iHeight;j++)
	{
		if(matIn.at<uchar>(j,0)==0)
			matOut.at<uchar>(j,0) = AUX_VALUE;

		if(matIn.at<uchar>(j, iWidth-1)==0)
			matOut.at<uchar>(j, iWidth-1) = AUX_VALUE;
	}

	for(int i=0;i<iWidth;i++)
	{
		if(matIn.at<uchar>(0, i)==0)
			matOut.at<uchar>(0, i) = AUX_VALUE;

		if(matIn.at<uchar>(iHeight-1, i)==0)
			matOut.at<uchar>(iHeight-1, i) = AUX_VALUE;
	}

	bool flagBG = true;

	while(flagBG)
	{
		int iCount=0;
		
		for(int j=0;j<iHeight;j++)
		{
			for(int i=0;i<iWidth;i++)
			{
				if((j!=0 && j!=iHeightM1 && i!=0 && i!=iWidthM1))
				{
					if( matOut.at<uchar>(j,i)==0)
					{

						uchar fN = matOut.at<uchar>(j-1,i);//*(pHole-iWidth);
						uchar fW = matOut.at<uchar>(j, i-1);//*(pHole-1);
						uchar fE = matOut.at<uchar>(j,i+1);//*(pHole+1);
						uchar fS = matOut.at<uchar>(j+1,i);//*(pHole+iWidth);

						if(fN == AUX_VALUE  || fW == AUX_VALUE  || fE == AUX_VALUE || fS == AUX_VALUE)
						{
							matOut.at<uchar>(j,i) = AUX_VALUE; 
							iCount++;
						}

					}
				}				
			}
		}					

		for(int j=iHeight-1;j>=0;j--)
		{
			for(int i=iWidth-1;i>=0;i--)
			{				
				if((j!=0 && j!=iHeightM1 && i!=0 && i!=iWidthM1))
				{

					if( matOut.at<uchar>(j,i)==0)
					{							
						uchar fN = matOut.at<uchar>(j-1,i);//*(pHole-iWidth);
						uchar fW = matOut.at<uchar>(j, i-1);//*(pHole-1);
						uchar fE = matOut.at<uchar>(j,i+1);//*(pHole+1);
						uchar fS = matOut.at<uchar>(j+1,i);//*(pHole+iWidth);					

						if(fN == AUX_VALUE  || fW == AUX_VALUE  || fE == AUX_VALUE || fS == AUX_VALUE)
						{
							matOut.at<uchar>(j,i) = AUX_VALUE; 
							iCount++;
						}

					}
				}				
			}
		}			
		if(iCount == 0) flagBG = false;
	}
    
	uchar*pcOut=NULL;
	    
	for(int j=0;j<iHeight;j++)
	{
        pcOut=matOut.ptr<uchar>(j);
        
		for(int i=0;i<iWidth;i++)
		{
		/*	uchar intVal = matOut.at<uchar>(j,i);

			if( intVal==AUX_VALUE ) 
				matOut.at<uchar>(j,i)=0;
			else
				matOut.at<uchar>(j,i)=255;
         */
            uchar intVal = *pcOut;
            
            if(intVal==AUX_VALUE)
                *pcOut=0;
            else
                *pcOut=255;
            
            pcOut++;            
		}
	}			
}