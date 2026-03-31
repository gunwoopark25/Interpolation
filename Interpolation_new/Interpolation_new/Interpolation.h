#pragma once
#include "Point2D.h"
class Interpolation
{
public:
	/*--- Variable ---*/
	int Degree;
	int Parameter;
	int R;
	int Number_POC;
	int Matrix_Size;
	int POC_Size;

	double Min_x;
	double Min_y;
	double Max_x;
	double Max_y;
	double dx;
	double dy;

	/*--- 1D ---*/
	double *u;
	double *l;

	/*--- 2D ---*/
	Point2D *POC;
	Point2D *CP;
	Point2D *Coordinate;
	Point2D *All_POC;
	double **Matrix;

	/*--- Function ---*/
	void inputData();
	void loadFile();
	void Chordlength();
	void Normalization();
};
