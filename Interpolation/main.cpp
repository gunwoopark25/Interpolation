#include <iostream>
#include <string>
#include <fstream>
#include "Interpolation.h"

using namespace std;

void main()
{
	Interpolation Interpolation3;

	Interpolation3.fileLoad();
	Interpolation3.normalization();
	Interpolation3.chord_length();
	Interpolation3.chord_length_normalization();
	Interpolation3.makeBernsteinMatrix();
	Interpolation3.GaussJordanElimination();
	Interpolation3.matrixMultiplication();
	Interpolation3.BezierCurve();
	Interpolation3.solveNormalization();
	Interpolation3.fileWrite();
}