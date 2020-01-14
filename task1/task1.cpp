// task1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>		// string io
#include <fstream>		// file open
#include <limits.h>		// max min int value
#include <algorithm>    // std::sort
#include <math.h>       // floor 
#include <iomanip>      // std::setprecision

using namespace std;

int main(int argc, char * argv[]) {


	try {
		// Проверяем начальные аргументы 
		if (argc != 2) {
			cerr << "Initial parameters missing...\n";
			throw 0;
		}
		setlocale(LC_CTYPE, "rus"); // не функционирует с потоком ввода


		int Arr[1000] = {}, ArrSize = 0, Max = INT_MAX, Min = INT_MIN, AvSum = 0;;
		float Avg = 0.0f, Perc90 = 0.0f, Perc50 = 0.0f;

		ifstream file(argv[1]);
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				Arr[ArrSize] = stoi(line);
				ArrSize++;
			}
			file.close();
		}
		else {
			cout << "Error during file IO\n";
			throw 0; // terminate with error
		}

		// считаем минимальное и максимальное значение массива
		sort(Arr, Arr + ArrSize);
		Min = Arr[0]; // минимальное
		Max = Arr[ArrSize - 1]; // максимальное 

		// считаем медиану
		if (ArrSize % 2 != 0) 
			Perc50 = (float)Arr[ArrSize / 2];	// нечетный массив 
		else 
			Perc50 = (float)(Arr[(ArrSize - 1) / 2] + Arr[ArrSize / 2]) / 2.0;	// четный массив

		// считаем перцентиль 90 по формуле Excel
		float Rank = 0.9f * (ArrSize - 1) + 1;
		float floorRank = floor(Rank);
		float modRank = Rank - floorRank;
		int Ind = (int)floorRank - 1;
		Perc90 = Arr[Ind] + modRank * (Arr[Ind + 1] - Arr[Ind]);

		// считаем среднее значение массива
		for (int i = 0; i < ArrSize; i++) {
			AvSum += Arr[i];
		}
		Avg = ((float)AvSum) / ArrSize;
		
		// change output format settings with member functions
		cout.setf(ios::fixed, ios::floatfield); // set fixed floating format
		cout.precision(2); // for fixed format, two decimal places

		cout << Perc90 << "\n" << Perc50 << "\n" << (float)Max << "\n" << (float)Min << "\n" << Avg << "\n";
	}
	catch (...) {		
		cerr << "Application ended with exeption (1)\n";
		//cin.get();
		return 1;
	}
	return 0;
}

