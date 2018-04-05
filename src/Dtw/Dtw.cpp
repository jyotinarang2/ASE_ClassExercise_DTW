
#include "Vector.h"
#include "Util.h"

#include "Dtw.h"
#include<iostream>
using namespace std;
CDtw::CDtw(void)
{

}

CDtw::~CDtw(void)
{

}

Error_t CDtw::init(int iNumRows, int iNumCols)
{
	//bIsInit = 1;
	ppfCost = new float*[iNumRows];
	for (int i = 0; i < iNumRows; i++)
		ppfCost[i] = new float[iNumCols];

	iTracebackPath = new int*[iNumRows];
	for (int i = 0; i < iNumRows; i++)
		iTracebackPath[i] = new int[iNumCols];


	for (int i = 0; i < iNumRows; i++) {
		for (int j = 0; j < iNumCols; j++) {
			ppfCost[i][j] = 0;
		}
	}
	for (int i = 0; i < iNumRows; i++) {
		for (int j = 0; j < iNumCols; j++) {
			iTracebackPath[i][j] = 0;
		}
	}
	iMatrixDimensions[MatrixDimension_t::kRow] = iNumRows;
	iMatrixDimensions[MatrixDimension_t::kCol] = iNumCols;
	return kNoError;
}

Error_t CDtw::reset()
{
	return kNoError;
}

Error_t CDtw::process(float **ppfDistanceMatrix)
{
	/*if (!bIsInit)
	{
		return kNotInitializedError;
	}*/
	ppfCost[0][0] = ppfDistanceMatrix[0][0];

	for (int i = 1; i < iMatrixDimensions[kCol]; i++)
	{
		ppfCost[0][i] = ppfCost[0][i - 1] + ppfDistanceMatrix[0][i];
		iTracebackPath[0][i] = 1;
	}

	for (int j = 1; j < iMatrixDimensions[kRow]; j++)
	{
		ppfCost[j][0] = ppfCost[j - 1][0] + ppfDistanceMatrix[j][0];
		iTracebackPath[j][0] = 2;
	}

	for (int i = 1; i < iMatrixDimensions[kRow]; i++)
	{
		for (int j = 1; j < iMatrixDimensions[kCol]; j++)
		{
			float min_value = computeMinimum(ppfCost[i - 1][j - 1], ppfCost[i][j - 1], ppfCost[i - 1][j]);
			iTracebackPath[i][j] = iCurrentDirection;

			ppfCost[i][j] = ppfDistanceMatrix[i][j] + min_value;
		}
	}
	
	return kNoError;
}
float CDtw::computeMinimum(float a, float b, float c)
{

	float min = a;
	iCurrentDirection = 0;

	if (a>b)
	{
		min = b;
		iCurrentDirection = 1;
	}

	if (c < min)
	{
		min = c;
		iCurrentDirection = 2;
	}
	return min;
}
int CDtw::getPathLength()
{

	int count = 0;
	int columns = iMatrixDimensions[kCol]-1;
	int rows = iMatrixDimensions[kRow]-1;
	while (columns > 0 || rows > 0) {
		if (iTracebackPath[rows][columns] == 0) {
			--columns; --rows;
			count++;
		}
		if (iTracebackPath[rows][columns] == 1) {
			--columns;

		}
		if (iTracebackPath[rows][columns] == 2) {
			--rows;
			count++;
		}
	}
	return count+1;
}

float CDtw::getPathCost() const
{
	return ppfCost[iMatrixDimensions[0]-1][iMatrixDimensions[1]-1];
}

Error_t CDtw::getPath(int **ppiPathResult) const
{
	return kNoError;
}

