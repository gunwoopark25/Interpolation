#include <iostream>
#include <string>
#include <fstream>
#include "Interpolation.h"

using namespace std;

void main()
{
    Interpolation Interpolation3;

    Interpolation3.inputData();
    Interpolation3.loadFile();
    Interpolation3.Chordlength();
    Interpolation3.Normalization();
    Interpolation3.makeBernsteinMatrix();
    Interpolation3.GaussJordanElimination();
    Interpolation3.MatrixMultiplication();
    Interpolation3.BezierCurve();
    Interpolation3.solveNormalization();
    Interpolation3.writePS();
}