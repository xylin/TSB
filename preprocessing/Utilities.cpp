#include "Utilities.h"

using namespace std;
using namespace cv;

double log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log( 2.0 );  
}

           
double EuclideanDist(const int& iX, const int& iY, const int& iXRef, const int& iYRef)
{
    return sqrt((double)(iX - iXRef)*(double)(iX - iXRef) + (double)(iY-iYRef)*(double)(iY-iYRef));
}


void FindEnclosing(const Mat& matDump, Rect& rctEncl)
{
	int iYMin(matDump.rows);
	int iXMin(matDump.cols);
	int iYMax(0);
	int iXMax(0);

	const uchar *pcDump = NULL;

	for(int y=0; y < matDump.rows; y++ )
	{
		pcDump = matDump.ptr<uchar>(y);

		for(int x=0; x < matDump.cols; x++ )
		{
			if((*pcDump)>0)
			{
				if(iXMin>x)
					iXMin=x;

				if(iYMin>y)
					iYMin=y;

				if(iXMax<x)
					iXMax=x;

				if(iYMax<y)
					iYMax=y;
			}
			pcDump++;
		}
	}

	rctEncl.x=iXMin;
	rctEncl.y=iYMin;
	rctEncl.width=iXMax-iXMin+1;
	rctEncl.height=iYMax-iYMin+1;
};


void VectorQuickSortAscendingWithAux(vector<double>& A, int low, int high, vector<int>& auxiliary_list)
{

	int low_pnt, high_pnt, pivot_position;
	double pivot;

	pivot_position = low + (high - low)/2;
	pivot = A[pivot_position];	

	if (A[low] > pivot)
	{
		QuickSortSwap(A[pivot_position], A[low]);
		QuickSortSwap(auxiliary_list[pivot_position], auxiliary_list[low]);
	}	

	if (A[high] < pivot)
	{
		QuickSortSwap(A[pivot_position], A[high]);
		QuickSortSwap(auxiliary_list[pivot_position], auxiliary_list[high]);		

		if (A[low] > A[pivot_position])
		{
			QuickSortSwap(A[pivot_position], A[low]);
			QuickSortSwap(auxiliary_list[pivot_position], auxiliary_list[low]);
		}
	}

	low_pnt = low;
	high_pnt = high;
	while (low_pnt < high_pnt)
	{
		do
		high_pnt--;
		while (A[high_pnt] > pivot);

		do
		low_pnt++;
		while (A[low_pnt] < pivot);		

		if (low_pnt <= high_pnt)
		{
			QuickSortSwap(A[high_pnt], A[low_pnt]);
			QuickSortSwap(auxiliary_list[high_pnt], auxiliary_list[low_pnt]);
		}
	}

	if (high_pnt + 1< high)
		VectorQuickSortAscendingWithAux(A, high_pnt + 1, high, auxiliary_list);

	if (low < high_pnt)
		VectorQuickSortAscendingWithAux(A, low, high_pnt, auxiliary_list);	

}


string SplitFilename (const string& str)
{
  size_t found;
  
  //cout << "Splitting: " << str << endl;
  
  found=str.find_last_of("/\\");

 // cout << " folder: " << str.substr(0,found) << endl;
 // cout << " file: " << str.substr(found+1) << endl;

  return str.substr(found+1);
}