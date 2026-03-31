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
	/*--- Reset Valiable ---*/
	Min_x = 1000;
	Min_y = 1000;
	Max_x = 0;
	Max_y = 0;

	Max_u = 0.0;
	t = 0.0;
	boundary = 0;
	Chord_length = 0.0;

	Degreep = 0;
	jp = 0;
	Degree_j = 0;
	Degree_jp = 0;

	nCr = 0.0;
	Set2 = 0.0;
	factor = 0.0;

	dx = 0.0;
	dy = 0.0;
	Sum_x = 0.0;
	Sum_y = 0.0;

	POC = nullptr;
	CP = nullptr;
	Coordinate = nullptr;
	All_POC = nullptr;
	Matrix = nullptr;
	u = nullptr;
	l = nullptr;
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

	Degree = 3;
	parameter = 11;
	R = 5;

	Number_POC = Degree + 1;
	Size = Number_POC * 2;
	POC_Size = Number_POC;
	for (int i = Number_POC - 1; i > 0; i--)
	{
		POC_Size = POC_Size + i;
	}

	/*--- Dynamic Memory Allocation ---*/
	POC = new Point2D[Degree];
	CP = new Point2D[Degree];
	Coordinate = new Point2D[POC_Size];
	All_POC = new Point2D[parameter];
	Matrix = new double* [Size-1];
	for (int i = 0; i < Number_POC; i++)
	{
		Matrix[i] = new double[Size - 1];
	}


	/*--- Writing POC at Point2D ---*/
	for (int i = 0; i <= Degree; i++)
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
	/*--- Min value ---*/
	for (int i = 0; i < Number_POC; i++)
	{
		if (Min_x > POC[i].x)
		{
			Min_x = POC[i].x;
		}
		if (Min_y > POC[i].y)
		{
			Min_y = POC[i].y;
		}
	}
	/*--- Max value ---*/
	for (int i = 0; i < Number_POC; i++)
	{
		if (Max_x < POC[i].x)
		{
			Max_x = POC[i].x;
		}
		if (Max_y < POC[i].y)
		{
			Max_y = POC[i].y;
		}
	}

	dx = Max_x - Min_x;
	dy = Max_y - Min_y;

	for (int i = 0; i < Number_POC; i++)
	{
		POC[i].x = (POC[i].x - Min_x) / dx;
		POC[i].y = (POC[i].y - Min_y) / dy;
	}
}

void Interpolation::chord_length()
{
	u = new double[Degree];
	l = new double[Degree - 1];

	u[0] = 0;
	for (int i = 1; i <= Degree; i++)
	{
		l[i-1] = sqrt(pow(POC[i].x - POC[i - 1].x, 2) + pow(POC[i].y - POC[i - 1].y, 2));
	}
	for (int i = 1; i < Number_POC; i++)
	{
		u[i] = u[i - 1] + l[i - 1];
	}
}

void Interpolation::chord_length_normalization()
{
	for (int i = 0; i < Number_POC; i++)
	{
		if (Max_u < u[i])
		{
			Max_u = u[i];
		}
	}
	for (int i = 0; i < Number_POC; i++)
	{
		u[i] = u[i] / Max_u;
	}
}


void Interpolation::makeBernsteinMatrix()
{
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = 0; j <= Degree; j++)
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

void Interpolation::GaussJordanElimination()
{
	/*--- Make IdentityMatrix ---*/
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = Number_POC; j <= Size - 1; j++)
		{
			if (i == j - Number_POC)
			{
				Matrix[i][j] = 1;
			}
			else
			{
				Matrix[i][j] = 0;
			}
		}
	}
	/*--- Check Matrix ---*/
	for (int i = 0; i < Number_POC; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			cout << Matrix[i][j] << " ";
		}
		cout << endl;
	}

	for (int i = 0; i < Number_POC; i++)
	{
		Set2 = Matrix[i][i];
		for (int j = 0; j < Size; j++)
		{
			Matrix[i][j] /= Set2;
		}

		for (int k = 0; k < Number_POC; k++)
		{
			if (k != i)
			{
				factor = Matrix[k][i];
				if (factor != 0) 
				{
					for (int j = 0; j < 2 * Number_POC; j++)
					{
						Matrix[k][j] -= Matrix[i][j] * factor;
					}
				}
			}
		}
	}
	
	for (int i = 0; i < Number_POC; i++)
	{
		for (int j = 0; j < Size; j++)
		{
			cout << Matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void Interpolation::matrixMultiplication()
{
	for (int i = 0; i < Number_POC; i++)
	{
		Sum_x = 0;
		Sum_y = 0;
		for (int j = 0; j < Number_POC; j++)
		{
			Sum_x += Matrix[i][Number_POC + j] * POC[j].x;
			Sum_y += Matrix[i][Number_POC + j] * POC[j].y;
		}
		CP[i].x = Sum_x;
		CP[i].y = Sum_y;
		cout << CP[i].x << " " << CP[i].y << endl;
	}
}
void Interpolation::BezierCurve()
{
	for (int k = 0; k <= parameter; k++)
	{
		t = (double)k / parameter;
		
		boundary = 0;

		for (int i = 1; i <= Degree; i++)
		{
			int n = Degree - i + 1;
			int start_X = boundary + 1;
			int end_X = boundary + n + 1;

			for (int X = start_X; X < end_X; X++)
			{
				Coordinate[X + n].x = (1 - t) * Coordinate[X - 1].x + t * Coordinate[X].x;
				Coordinate[X + n].y = (1 - t) * Coordinate[X - 1].y + t * Coordinate[X].y;
			}
			boundary += Degree + 1 - i + 1;
		}

		int last_X = POC_Size - 1;
		All_POC[k].x = Coordinate[last_X].x;
		All_POC[k].y = Coordinate[last_X].y;
	}
}

void Interpolation::solveNormalization()
{
	for (int i = 0; i <= Degree; i++)
	{
		POC[i].x = POC[i].x * dx + Min_x;
		POC[i].y = POC[i].y * dy + Min_y;
		CP[i].x = CP[i].x * dx + Min_x;
		CP[i].y = CP[i].y * dy + Min_y;
	}
	for (int i = 0; i <= parameter; i++)
	{
		All_POC[i].x = All_POC[i].x * dx + Min_x;
		All_POC[i].y = All_POC[i].y * dy + Min_y;
	}
	
}

void Interpolation::fileWrite()
{
	ofstream txtFile("CP.txt");
	ofstream psFile("Curve.ps");

	/*--- Check File ---*/
	if (!txtFile || !psFile)
	{
		/*--- When can't file output ---*/
		std::cout << "You can't load this file" << endl;
	}

	/*--- Post Script Header ---*/
	psFile << "%!PS" << endl;

	/*--- Drawing Cp Line at ps---*/
	psFile << "newpath" << endl;
	for (int i = 0; i < Number_POC; i++)
	{
		if (i == 0)
		{
			psFile << CP[i].x << " " << CP[i].y << "moveto" << endl;
		}
		else
		{
			psFile << CP[i].x << " " << CP[i].y << "lineto" << endl;
		}
	}
	psFile << "stroke" << endl;

	/*--- Write Cp at txt ---*/
	for (int i = 0; i <= Degree; i++)
	{
		txtFile << CP[i].x << " " << CP[i].y << endl;
	}
	
	/*--- Drawing Circle at Cp ---*/
	for (int i = 0; i <= Degree; i++)
	{
		psFile << "newpath" << endl;
		if (i= 0 || i == Degree)
		{
			psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
			psFile << "fill" << endl;
		}
		else
		{
			psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
			psFile << "stroke" << endl;
		}
	}

	/*--- Drawing input POC Line at ps ---*/
	psFile << "newpath" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		if (i == 0)
		{
			psFile << POC[i].x << " " << POC[i].y << " moveto" << endl;

		}
		else
		{
			psFile << POC[i].x << " " << POC[i].y << " lineto" << endl;
		}
	}
	psFile << "stroke" << endl;

	/*--- Drawing Circle at POC ---*/
	for (int i = 0; i <= Degree; i++)
	{
		psFile << "newpath" << endl;
		if (i = 0 || i == Degree)
		{
			psFile << POC[i].x << " " << POC[i].y << " " << R << " " << "0 360 arc" << endl;
			psFile << "fill" << endl;
		}
		else
		{
			psFile << POC[i].x << " " << POC[i].y << " " << R << " " << "0 360 arc" << endl;
			psFile << "stroke" << endl;
		}
	}

	/*--- Drawing Bezier Curve ---*/
	psFile << "newpath" << endl;
	for (int i = 0; i <= parameter; i++)
	{
		txtFile << All_POC[i].x << " " << All_POC[i].y << endl;

		if (i == 0)
		{
			/*--- Start Point ---*/
			psFile << All_POC[i].x << " " << All_POC[i].y << " moveto" << endl;
		}
		else
		{
			/*--- Line ---*/
			psFile << All_POC[i].x << " " << All_POC[i].y << " lineto" << endl;
		}
	}
	psFile << "stroke" << endl;
	psFile << "showpage" << endl;

	txtFile.close();
	psFile.close();
}