
#include "Utilities.h"

#include "RegionLabelling.h"


void RegionLabelling2D(const Mat& maskBGD, vector< vector<cv::Point> >& vvpRegions)
{
	int iHeight= maskBGD.rows;
	int iWidth = maskBGD.cols;
	int iFrameSize = iWidth * iHeight;
    
	STACK_PIXEL myStackPixel;
    
	Mat labelledImg;
    
	labelledImg.create(iHeight, iWidth, CV_32SC1);
	labelledImg.setTo(0);
    
	int iLabelNumber = 1; // first label to e assigned after this value
    
	_PIXEL myPixel;
	_PIXEL adjacentPixel;
    
	int iNumRegs  = 0;
	int iTotalRegions = 0;
    
	for(int j = 0;j < iHeight; j++)
	{
		for(int i = 0; i < iWidth; i++)
		{
			if( maskBGD.at<uchar>(j,i)>0 && labelledImg.at<int>(j,i)==0)
			{
				myPixel.i = i;
				myPixel.j = j;
                
				myStackPixel.push(myPixel); // put first point in the stack
                
				vector<cv::Point> vpLabelled;
                
				vpLabelled.push_back(cv::Point(i, j));
                
				labelledImg.at<unsigned int>(j, i) = ++iLabelNumber;
                
				iTotalRegions++;
				iNumRegs++;
                
				int iNumPixels = 1;
                
				while( !myStackPixel.empty() )
				{
					// get the point freom the stack
					myPixel = myStackPixel.top();
					myStackPixel.pop();
                    
					// find its neighbours, if it is forground and not labelled before, push into the stack
					for(int jj = myPixel.j - 1; jj <= myPixel.j + 1; jj++)
					{
						for(int ii = myPixel.i - 1; ii <= myPixel.i + 1; ii++)
						{
							if (!( ii < 0 || ii >= iWidth || jj < 0 || jj >= iHeight) && !(ii == myPixel.i && jj == myPixel.j))
							{
								if ( maskBGD.at<uchar>(jj,ii)>0 && labelledImg.at<unsigned int>(jj,ii)==0 )
								{
                                    
									adjacentPixel.i = ii;
									adjacentPixel.j = jj;
                                    
									myStackPixel.push(adjacentPixel);
                                    
									vpLabelled.push_back(cv::Point(ii, jj));
									
									labelledImg.at<unsigned int>(jj,ii) = iLabelNumber;
									
									iNumPixels++;									
								}
							}
						}
					}						
				}// end while
                
				vvpRegions.push_back(vpLabelled);			
			}			
		}
	}
}
