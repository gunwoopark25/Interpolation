#include "Interpolation.h"
#include "Point2D.h"
#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

using namespace std;

void Interpolation::inputData()
{
	/*--- 입력해야하는 데이터 ---*/
	Degree = 3;
	Parameter = 10;
	R = 5;

	/*--- 계산된 데이터 ---*/
	Number_POC = Degree + 1;
	Matrix_Size = Number_POC * 2;

	POC_Size = Number_POC;
	for (int i = Degree; i > 0; i--)
	{
		POC_Size = POC_Size + 1;
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
	All_POC = new Point2D[Parameter];

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
	}

	dx = Max_x - Min_x;
	dy = Max_y - Min_y;

	/*--- POC 정규화 ---*/
	for (int i = 0; i <= Degree; i++)
	{
		POC[i].x = (POC[i].x - Min_x) / dx;
		POC[i].y = (POC[i].y - Min_y) / dy;
	}
}