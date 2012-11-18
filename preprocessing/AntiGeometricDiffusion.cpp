#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"


void AntiGeometricDiffusionInSlice(const float* pfInput, float fDelta, int iMaxIterations, int iType, int iHeight, int iWidth, float*pfOutput)
{   
    float *pfOldSliceData=NULL;
    float* pfNewSliceData =NULL;   
   
    int i, j;       

    float fXpixel = 1;
    float fYpixel = 1;
    float fRXPixelSq = 1.0f;
    float fRYPixelSq = 1.0f;

    float fRXPixel = 1.0f / fXpixel;
    float fRYPixel = 1.0f / fYpixel;
   
    fRXPixelSq = fRXPixel * fRXPixel;
    fRYPixelSq = fRYPixel * fRYPixel;

    int iFrameSize = iWidth * iHeight;       

    pfOldSliceData = new float[iFrameSize];

    float *pfTempOld = pfOldSliceData;

    const float *pOrigData = pfInput;

    int iCounter = iFrameSize;
    do
    {
        *pfTempOld++ = *pOrigData++;
    }while(--iCounter);

    pfNewSliceData = new float[iFrameSize];

    int iIndexNorthWest =0;
    int iIndexNorth = 0;
    int iIndexNorthEast = 0;

    int iIndexWest = 0;
    int iIndexEast = 0;

    int iIndexSouthWest = 0;
    int iIndexSouth = 0;
    int iIndexSouthEast = 0;       

    for(int iIter = 0; iIter < iMaxIterations; iIter++ )
    {       
        int iIndexPoint = iWidth;                               

        for( j = 1; j < iHeight-1; j++ )
        {   
            iIndexPoint += 1;
            for( i = 1; i < iWidth-1; i++ )
            {                                           
                iIndexNorth = iIndexPoint - iWidth;
                iIndexSouth = iIndexPoint + iWidth;
                iIndexNorthEast = iIndexNorth + 1;
                iIndexEast = iIndexPoint +1;
                iIndexSouthEast = iIndexSouth +1;                       
                iIndexNorthWest = iIndexNorth - 1;
                iIndexWest = iIndexPoint -1;
                iIndexSouthWest = iIndexSouth-1;


                float& fValue_point     = pfOldSliceData[iIndexPoint];   // it is to be updated                       
                float& fValue_south     = pfOldSliceData[iIndexSouth];                       
                float& fValue_north     = pfOldSliceData[iIndexNorth];                       
                float& fValue_west      = pfOldSliceData[iIndexWest];                       
                float& fValue_east      = pfOldSliceData[iIndexEast];                       
                float& fValue_southwest = pfOldSliceData[iIndexSouthWest];                       
                float& fValue_southeast = pfOldSliceData[iIndexSouthEast];                       
                float& fValue_northeast = pfOldSliceData[iIndexNorthEast];                       
                float& fValue_northwest = pfOldSliceData[iIndexNorthWest];

                float fLaplacianX  = 0.5f*(fRXPixel * (fValue_east - fValue_west));
                float fLaplacianXX = fRXPixelSq * (fValue_east - 2.0f * fValue_point + fValue_west);   
                float fAValue = fLaplacianXX * fLaplacianX * fLaplacianX ;

                float fLaplacianY  = 0.5f * (fRYPixel  * (fValue_south -  fValue_north ));
                float fLaplacianYY = fRYPixelSq * (fValue_south - 2.0f * fValue_point + fValue_north);
                float fCValue = fLaplacianYY * fLaplacianY * fLaplacianY ;

                float fLaplacianXY = ( 0.25f * fRXPixel * fRYPixel ) * ( (fValue_southeast + fValue_northwest)
                    - ( fValue_southwest + fValue_northeast));

                float fBValue = fLaplacianX * fLaplacianY * fLaplacianXY;

                float fLaplacian = 0;
               
                if(iType==0)
                {
                    fLaplacian = (fAValue + 2.0f * fBValue + fCValue ) / ((fLaplacianX * fLaplacianX ) + (fLaplacianY * fLaplacianY) + 1.0e-8f);
                }
                else
                {
                    fLaplacian = ((fLaplacianY * fLaplacianY * fLaplacianXX) - ( 2.0f * fLaplacianX * fLaplacianY * fLaplacianXY)
                        + ( fLaplacianX * fLaplacianX * fLaplacianYY)) /( (fLaplacianX * fLaplacianX) + (fLaplacianY * fLaplacianY) + 1.0e-8f);
                }

                pfNewSliceData[iIndexPoint] = (fValue_point + fLaplacian * fDelta) ;// unbiased anisotropic diffusion                                               
               
                iIndexPoint++;
            }

            iIndexPoint += 1;
        }
        memcpy(pfOldSliceData, pfNewSliceData, iFrameSize * sizeof(float) );
    }
       
    pfTempOld = pfOldSliceData;

    int iCount(0);

    for( j = 0; j < iHeight; j++ )
    {
        for( i = 0; i < iWidth; i++ )
        {
            pfOutput[iCount] = *pfTempOld;                           
           
            iCount++;
            pfTempOld++;
        }
    }
   
    delete [] pfNewSliceData;
    delete [] pfOldSliceData;
}
