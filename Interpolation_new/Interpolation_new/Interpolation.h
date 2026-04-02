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
	double Max_u;
	double dx;
	double dy;

	int Degreep;
	int jp;
	int Degree_jp;
	int Degree_j;
	double nCr;

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
	void makeBernsteinMatrix();
	void GaussJordanElimination();
	void MatrixMultiplication();
	void BezierCurve();
	void solveNormalization();
	void writePS();
};
