#include <iostream>
#include <string>
#include <fstream>
#include "Interpolation.h"

using namespace std;

int main()
{
    Interpolation Interpolation3;

    Interpolation3.inputData();
    Interpolation3.loadFile();
    Interpolation3.Chordlength();
    Interpolation3.Normalization();

    return 0;
}