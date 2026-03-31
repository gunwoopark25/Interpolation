#include "Interpolation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include<fstream>
#include<cmath>

void Interpolation::inputData()
{
	/*--- Input Data---*/
	Degree = 3;
	Parameter = 10;
	R = 5;

	/*--- Calculated Data ---*/
	Number_POC = Degree + 1;
}