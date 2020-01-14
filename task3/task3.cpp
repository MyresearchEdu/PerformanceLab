// task3.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string> // string io
#include <fstream> // file io
#include <algorithm>    // std::sort
#include <ctype.h> // isspace
#include <cctype>

using namespace std;

// Убрать пробелы из пути к файлу в начале и в конце строки
static inline void TrimString2(string &s); 

// Читаем файл в массив и сохраняем его длинну
int ParseFile(string path, float(&Arr)[16]); 

int main(int argc, char * argv[]) {

	try {
		// Проверяем начальные аргументы 
		if (argc != 2) {
			cerr << "Initial parameters missing...\n";
			throw 0;
		}

		float Arr[6][16] = {};
		int ic[5] = {};

		string DATA_DIR = argv[1];
		TrimString2(DATA_DIR); // Убрать пробелы из пути к файлу в начале и в конце строки
		if (DATA_DIR.back() != '\\') DATA_DIR += "\\"; // Если в конце пути не хватает косой черты то добавить ее
		
		// Извлекаем содержимое файлов в массивы
		for (int i = 0; i < 5; i++){
			string falename = DATA_DIR + "Cash" + to_string(i+1) + ".txt";
			ic[i] = ParseFile(falename, Arr[i]);
		}

		// Проверяем целостность входных данных
		for (int i = 1; i < 5; i++) {
			if (ic[i] == ic[0]) continue;
			else {
				cout << "Initial data missing some information. Data damage possible.\n" << endl;			
				throw 0;
			}
		}

		// Считаем количество посетителетей на всех кассах магазина для определенного интервала
		for (int i = 0; i < ic[0]; i++) {
			Arr[5][i] = Arr[0][i] + Arr[1][i] + Arr[2][i] + Arr[3][i] + Arr[4][i];
		}

		// Вычисляем интервал с наибольшим количеством посетителей по длинне очереди к кассе
		float* Max = max_element(Arr[5], Arr[5] + ic[0]);
				
		// Определяем позицию интервала через указатель на массив
		float* ptr = Arr[5];
		for (int i = 0; i < ic[0]; i++) {
			if (ptr != Max) ptr++;
			else{
				int time = 10 + (i + 1) *0.5,
					hour = floor(time),
					minute = 60 * (time - floor(time));

				cout << "Interval = " << i + 1 << ", Time = " << hour << "." << minute << " hours [10.00 - 18.00], Visitors (meters): " << *Max << "\n";
				break;
			}
		}
	}
	catch (...) {		
		cerr << "Application ended with exeption (1)\n";
		//cin.get();
		return 1;
	}
	return 0;
}

// Убрать пробелы из пути к файлу в начале и в конце строки
static inline void TrimString2(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
	}));

	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// Читаем файл в массив и сохраняем его длинну
int ParseFile(string path, float(&Arr)[16]) {
	int iIntv = 0;
	ifstream file(path);
	if (file.is_open()) {
		string line = "";
		while (getline(file, line)) {
			Arr[iIntv] = stof(line);
			iIntv++;
		}
		file.close();
	}
	else {
		cout << "Error during file IO\n";
	}
	return iIntv;
}