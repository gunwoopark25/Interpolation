#include "Interpolation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

/*--- Consturcture ---*/
Interpolation::Interpolation()
{
	/*--- Input Variable ---*/
	Degree = 3;


	/*--- Calculate Variable ---*/
	Number_POC = Degree + 1;

	POC = nullptr;
	CP = nullptr;
}
/*--- Destructor ---*/
Interpolation::~Interpolation()
{

}

/*--- Define function ---*/
/*--- File Loading ---*/
void Interpolation::fileLoad()
{
	/*--- File Open ---*/
	ifstream readFile("points.txt");

	if (!readFile)
	{
		cout << "You can't load this file" << endl;
	}

	/*--- Dynamic Memory Allocation ---*/
	POC = new Point2D[Number_POC];
	CP = new Point2D[Number_POC];
	Matrix = new double* [Number_POC];
	for (int i = 0; i < Number_POC; i++)
	{
		Matrix[i] = new double[Number_POC];
	}

	/*--- Writing POC at Point2D ---*/
	for (int i = 0; i < Number_POC; i++)
	{
		readFile >> POC[i].x;
		readFile >> POC[i].y;
	}
	/*--- Closing .txt File ---*/
	readFile.close();
}

/*--- Coordinate Normalize ---*/
void Interpolation::normalization()
{
	/*--- Save Variable ---*/
	Min_x = POC[0].x;
	Min_y = POC[0].y;
	Max_x = POC[Degree].x;
	Max_y = POC[Degree].y;

	dx = Max_x - Min_x;
	dy = Max_y - Min_y;
	x_y = sqrt(dx * dx + dy * dy);

	for (int i = 0; i < Number_POC; i++)
	{
		POC[i].x = (POC[i].x - Min_x) / x_y;
		POC[i].y = (POC[i].y - Min_y) / x_y;
	}
}

void Interpolation::solveNormalization()
{
	for (int i = 0; i < Number_POC; i++)
	{
		CP[i].x = CP[i].x * x_y + Min_x;
		CP[i].y = CP[i].y * x_y + Min_y;
	}
}

void Interpolation::chord_length()
{
	u = new double[Number_POC];
	l = new double[Number_POC];

	u[0] = 0;
	for (int i = 1; i < Number_POC; i++)
	{
		l[i-1] = sqrt(pow(POC[i].x - POC[i - 1].x, 2) + pow(POC[i].y - POC[i - 1].y, 2));
	}
	for (int i = 1; i < Number_POC; i++)
	{
		u[i] = u[i - 1] + l[i - 1];
	}
}


void Interpolation::makeBernsteinMatrix()
{
	for (int i = 0; i < Number_POC; i++)
	{
		for (int j = 0; j < Number_POC; j++)
		{
			/*--- Calculate nCr ---*/
			Degreep = 1;
			jp = 1;
			Degree_jp = 1;

			for (int k = 1; k <= Degree; k++)
			{
				Degreep *= k;
			}
			for (int k = 1; k <= j; k++)
			{
				jp *= k;
			}
			
			Degree_j = Degree - j;
			for (int k = 1; k <= Degree_j; k++)
			{
				Degree_jp *= k;
			}
			nCr = Degreep / (jp * Degree_jp);

			/*--- Bernstein Polynomial ---*/
			Matrix[i][j] = nCr * pow(1 - u[i], Degree - j) * pow(u[i], j);
		}
	}
}