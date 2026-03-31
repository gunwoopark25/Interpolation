#include <iostream>
#include <string>
#include <fstream>
#include "Interpolation.h"

using namespace std;

int main()
{
	Interpolation Interpolation3;

	Interpolation3.fileLoad();
	Interpolation3.normalization();
	Interpolation3.chord_length();
	Interpolation3.makeBernsteinMatrix();

	return 0;
}