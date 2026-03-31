#pragma once
#include"Point2D.h"

class Interpolation
{
public:
	/*--- Constructor ---*/
	Interpolation();
	/*--- Destructor ---*/
	~Interpolation();

	/*--- Variable ---*/
	int Degree;
	int Number_POC;
	double Chord_length;

	int Degreep;
	int jp;
	int Degree_j;
	int Degree_jp;

	double nCr;

	/*--- For Normalization ---*/
	double Min_x;
	double Min_y;
	double Max_x;
	double Max_y;
	double dx;
	double dy;
	double x_y;
	
	/*--- 2D Variable ---*/
	Point2D* POC;
	Point2D* CP;
	double **Matrix;
	double* u;
	double* l;

	/*--- Function ---*/
	void fileLoad(); // .txt File Loading
	void normalization();
	void solveNormalization();
	void chord_length();
	void makeBernsteinMatrix();
};
