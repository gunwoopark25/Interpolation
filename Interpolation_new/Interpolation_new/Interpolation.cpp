#include "Interpolation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include<fstream>
#include<cmath>

using namespace std;

void Interpolation::inputData()
{
	/*--- Input Data---*/
	Degree = 3;
	Parameter = 10;
	R = 5;

	/*--- Calculated Data ---*/
	Number_POC = Degree + 1;
	Size = Number_POC * 2;
	
	POC_Size = Number_POC;
	for (int i = Degree; i > 0; i--)
	{
		POC_Size = POC_Size + 1;
	}
}