#include "Interpolation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

Interpolation::Interpolation()
{
	inputData();
}

Interpolation::~Interpolation()
{
	writePS();
}

void Interpolation::inputData()
{
	/*--- 입력해야하는 데이터 ---*/
	Degree = 3;
	Parameter = 100;
	R = 5;

	/*--- 계산된 데이터 ---*/
	Number_POC = Degree + 1;
	Matrix_Size = Number_POC * 2;

	POC_Size = Number_POC;
	for (int i = Degree; i > 0; i--)
	{
		POC_Size = POC_Size + i;
	}
}

void Interpolation::loadFile()
{
	/*--- 파일 로딩 ---*/
	ifstream readFile("points.txt");
	if (!readFile)
	{
		cout << "You can't load this file." << endl;
	}

	/*--- 2D 동적할당 ---*/
	POC = new Point2D[Number_POC];
	CP = new Point2D[Number_POC];
	Coordinate = new Point2D[POC_Size];
	All_POC = new Point2D[Parameter+1];

	/*--- Matrix 동적할당 ---*/
	Matrix = new double *[Number_POC];
	for (int i = 0; i <= Degree; i++)
	{
		Matrix[i] = new double[Matrix_Size];
	}

	/*--- POC에 txt파일에 있는 POC 저장 ---*/
	cout << "POC(Input Data)" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		/*--- 저장 ---*/
		readFile >> POC[i].x;
		readFile >> POC[i].y;

		/*--- 터미널에 출력 ---*/
		cout << POC[i].x << " " << POC[i].y << endl;
	}

	/*--- 파일 닫기 ---*/
	readFile.close();
}

void Interpolation::Chordlength()
{
	/*--- u,l 1차원 동적할당 ---*/
	u = new double[Number_POC];
	l = new double[Degree];

	/*--- 컴퓨터이용선형설계 73pg 개념 ---*/
	u[0] = 0;
	for (int i = 1; i <= Degree; i++)
	{
		l[i - 1] = sqrt(pow(POC[i].x - POC[i - 1].x, 2) + pow(POC[i].y - POC[i - 1].y, 2));
	}
	for (int i = 1; i <= Degree; i++)
	{
		u[i] = u[i - 1] + l[i - 1];
	}
}

void Interpolation::Normalization()
{
	/*--- 변수 초기화 : 값 초기화해주고 계산해야 값이 정확함 ---*/
	Min_x = POC[0].x;
	Min_y = POC[0].y;
	Max_x = POC[0].x;
	Max_y = POC[0].y;
	/*--- 최대/최소값 찾기 ---*/
	for (int i = 0; i <= Degree; i++)
	{
		/*--- 최솟값 찾기 ---*/
		if (Min_x > POC[i].x)
		{
			Min_x = POC[i].x;
		}
		if (Min_y > POC[i].y)
		{
			Min_y = POC[i].y;
		}
		/*--- 최댓값 찾기 ---*/
		if (Max_x < POC[i].x)
		{
			Max_x = POC[i].x;
		}
		if (Max_y < POC[i].y)
		{
			Max_y = POC[i].y;
		}
		if (Max_u < u[i])
		{
			Max_u = u[i];
		}
	}

	dx = Max_x - Min_x;
	dy = Max_y - Min_y;

	for (int i = 0; i <= Degree; i++)
	{
		/*--- POC 정규화 ---*/
		POC[i].x = (POC[i].x - Min_x) / dx;
		POC[i].y = (POC[i].y - Min_y) / dy;

		/*--- Chord Length 정규화 ---*/
		u[i] = u[i] / Max_u;
	}
}

void Interpolation::makeBernsteinMatrix()
{
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = 0; j <= Degree; j++)
		{
			int Degreep = 1;
			int jp = 1;
			int Degree_jp = 1;

			for (int k = 1; k <= Degree; k++)
			{
				Degreep *= k;
			}
			for (int k = 1; k <= j; k++)
			{
				jp *= k;
			}

			int Degree_j = Degree - j;
			for (int k = 1; k <= Degree_j; k++)
			{
				Degree_jp *= k;
			}
			double nCr = Degreep / (jp * Degree_jp);

			/*--- Bernstein Polynomial ---*/
			Matrix[i][j] = nCr * pow(1 - u[i], Degree - j) * pow(u[i], j);
		}
	}
}

void Interpolation::GaussJordanElimination()
{
	/*--- 단위 행렬 생성 ---*/
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = Degree + 1; j <= Matrix_Size - 1; j++)
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

	/*--- 역행렬을 만들기 위한 행렬 ---*/
	cout << "Matrix" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = 0; j <= Matrix_Size - 1; j++)
		{
			cout << Matrix[i][j] << " ";
		}
		cout << endl;
	}

	/*--- Gauss Jordan Elimination ---*/
	for (int i = 0; i <= Degree; i++)
	{
		double Set2 = Matrix[i][i];
		for (int j = 0; j <= Matrix_Size - 1; j++)
		{
			Matrix[i][j] /= Set2;
		}

		for (int k = 0; k <= Degree; k++)
		{
			if (k != i)
			{
				double factor = Matrix[k][i];
				if (factor != 0)
				{
					for (int j = 0; j <= Matrix_Size - 1; j++)
					{
						Matrix[k][j] -= Matrix[i][j] * factor;
					}
				}
			}
		}
	}

	/*--- 가우스조던소거법 완료된 행렬 출력 ---*/
	cout << "Calculated Matrix" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		for (int j = 0; j <= Matrix_Size - 1; j++)
		{
			cout << Matrix[i][j] << " ";
		}
		cout << endl;
	}
}

void Interpolation::MatrixMultiplication()
{
	cout << "CP Coordinate" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		/*--- 변수 초기화 ---*/
		double Sum_x = 0;
		double Sum_y = 0;

		/*--- 행렬 곱 ---*/
		for (int j = 0; j <= Degree; j++)
		{
			Sum_x += Matrix[i][Number_POC + j] * POC[j].x;
			Sum_y += Matrix[i][Number_POC + j] * POC[j].y;
		}

		/*--- 값 저장 ---*/
		CP[i].x = Sum_x;
		CP[i].y = Sum_y;

		cout << CP[i].x << " " << CP[i].y << endl;
	}
}

void Interpolation::BezierCurve()
{
	for (int k = 0; k <= Parameter; k++)
	{
		double t = (double)k / Parameter;

		for (int i = 0; i < Number_POC; i++)
		{
			Coordinate[i].x = CP[i].x;
			Coordinate[i].y = CP[i].y;
		}

		int boundary = 0;

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

			boundary += (Number_POC - i + 1);
		}

		int last_X = POC_Size - 1;
		All_POC[k].x = Coordinate[last_X].x;
		All_POC[k].y = Coordinate[last_X].y;
	}

	cout << "All POC Coordinate" << endl;
	for (int i = 0; i <= Parameter; i++)
	{
		cout << All_POC[i].x << " " << All_POC[i].y << endl;
	}
}

void Interpolation::solveNormalization()
{
	for (int i = 0; i <= Degree; i++)
	{
		/*--- POC 정규화 해제 ---*/
		POC[i].x = POC[i].x * dx + Min_x;
		POC[i].y = POC[i].y * dy + Min_y;

		/*--- CP 정규화 해제 ---*/
		CP[i].x = CP[i].x * dx + Min_x;
		CP[i].y = CP[i].y * dy + Min_y;
	}

	for (int i = 0; i <= Parameter; i++)
	{
		/*--- All POC 정규화 해제 ---*/
		All_POC[i].x = All_POC[i].x * dx + Min_x;
		All_POC[i].y = All_POC[i].y * dy + Min_y;
	}

	/*--- 모든 Point 출력 ---*/
	cout << "--- POC Coordinate ---" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		cout << POC[i].x << " " << POC[i].y << endl;
	}
	cout << "--- CP Coordinate ---" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		cout << CP[i].x << " " << CP[i].y << endl;
	}
	cout << "--- All POC Coordinate ---" << endl;
	for (int i = 0; i <= Parameter; i++)
	{
		cout << All_POC[i].x << " " << All_POC[i].y << endl;
	}
}

void Interpolation::writePS()
{
	ofstream psFile("Curve.ps");

	if (!psFile)
	{
		std::cout << "You can't load this file" << endl;
	}
	/*--- ps 헤더 ---*/
	psFile << "%!PS" << endl;

	/*--- CP 라인 그리기 ---*/
	psFile << "newpath" << endl;
	for (int i = 0; i <= Degree; i++)
	{
		if (i == 0)
		{
			psFile << CP[i].x << " " << CP[i].y << " " << "moveto" << endl;
		}
		else
		{
			psFile << CP[i].x << " " << CP[i].y << " " << "lineto" << endl;
		}
	}
	psFile << "stroke" << endl;

	/*--- CP 원 그리기 ---*/
	for (int i = 0; i <= Degree; i++)
	{
		psFile << "newpath" << endl;
		if (i == 0)
		{
			psFile << CP[i].x << " " << CP[i].y << " " << R << " " << "0 360 arc" << endl;
			psFile << "fill" << endl;
		}
		else if (i == Degree)
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

	/*--- All POC Line 그리기 ---*/
	psFile << "newpath" << endl;
	for (int i = 0; i <= Parameter; i++)
	{
		if (i == 0)
		{
			psFile << All_POC[i].x << " " << All_POC[i].y << " " << "moveto" << endl;
		}
		else
		{
			psFile << All_POC[i].x << " " << All_POC[i].y << " " << "lineto" << endl;
		}
	}
	psFile << "stroke" << endl;

	/*--- POC 원 그리기 ---*/
	for (int i = 0; i <= Degree; i++)
	{
		psFile << "newpath" << endl;
		psFile << POC[i].x << " " << POC[i].y << " " << R << " " << "0 360 arc" << endl;
		psFile << "fill" << endl;
	}

	psFile << "showpage" << endl;

	psFile.close();
}