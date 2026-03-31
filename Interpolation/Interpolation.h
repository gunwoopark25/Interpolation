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
	int parameter;
	double t;
	int boundary;
	int Number_POC;
	int Size;
	double Chord_length;
	int R;

	int Degreep;
	int jp;
	int Degree_j;
	int Degree_jp;

	double nCr;
	double Set2;
	double factor;

	/*--- For Normalization ---*/
	double Min_x;
	double Min_y;
	double Max_x;
	double Max_y;
	double dx;
	double dy;
	double Sum_x;
	double Sum_y;
	int POC_Size;


	/*--- 2D Variable ---*/
	Point2D* POC;
	Point2D* CP;
	Point2D* Coordinate;
	Point2D* All_POC;
	double** Matrix;
	double* u;
	double Max_u;
	double* l;

	/*--- Function ---*/
	void fileLoad(); // .txt File Loading
	void normalization();
	void chord_length();
	void chord_length_normalization();
	void makeBernsteinMatrix();
	void GaussJordanElimination();
	void matrixMultiplication();
	void solveNormalization();
	void fileWrite();
	void BezierCurve();
};
