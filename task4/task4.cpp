// task4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>		// file open
#include <algorithm>    // std::sort
#include <sstream>      // std::istringstream

using namespace std;

struct MostVisitedInterval {
	int a;
	int d;
};

int DetermineMostVisitedInterval(int arr[], int dep[], int iV, MostVisitedInterval MG[])
{
	// Сортируем время посещения по возрастанию для сравнения 
	sort(arr, arr + iV);
	sort(dep, dep + iV);
	
	// guests_in - количество гостей в данный момент 
	int guests_in = 1, max_guests = 1;
	int i = 1, j = 0;

	while (i < iV && j < iV)
	{
		// Если ближайшее по времени событие - въезд гостя, то увеличиваем количество посетителей 
		if (arr[i] <= dep[j])
		{
			guests_in++;

			// Фиксируем максимум посетителей, если он был
			// arrl[i] != exit[j] - исключить ситуацию когда въезд и выезд происходят одновременно, новый человек присутствует менее 1-ой секунды
			if (guests_in > max_guests && arr[i] != dep[j])
			{
				max_guests = guests_in;
			}
			i++;  // Рассматриваем следующий въезд 
		}
		else // Если ближайшее по времени событие - выезд гостя, то уменьшаем количество посетителей
		{    
			guests_in--;

			j++;	 // Рассматриваем следующий выезд 
		}
	}

	guests_in = 1; i = 1; j = 0;
	int iGT = 0;

	// Запоминаем интервалы с наибольшим количеством посетителей 
	while (j < iV) // проверяем всех посетителей до их полного выезда
	{
		// Если ближайшее по времени событие - въезд гостя, то увеличиваем количество посетителей 
		if (arr[i] <= dep[j])
		{
			guests_in++;

			// Запоминаем интервалы с наибольшим количеством посетителей 
			if (guests_in == max_guests) // въехал, количество жильцов увеличилось
			{
				MG[iGT].a = arr[i];
			}
			i++;  // Рассматриваем следующий въезд
		}
		else // Если ближайшее по времени событие - выезд гостя, то уменьшаем количество посетителей
		{
			guests_in--;

			// Запоминаем интервалы с наибольшим количеством посетителей
			if (guests_in == max_guests - 1) // выехал, количество жильцов уменьшилось
			{
				MG[iGT].d = dep[j];
				iGT++;
			}
			j++;	 // Рассматриваем следующий выезд
		}
	}
	//cout << "Максимальное количество гостей = " << max_guests << endl;

	return iGT;
}

// Переводим время в минуты для сравнения
int CalculateMinutes(string str) {
	string word = "";
	int time = 0;
	for (auto i : str) {
		if (i == ':') {
			time = stoi(word)*60; // определение часов посещения
			word = "";
		}
		else word = word + i;
	}
	time += stof(word); // уточнение минут для времени посещения
	return time;
}

// Извлекаем значения из строки сохраненной в файле
void ParseString(string str, int& x, int& y) {
	istringstream ss(str);		// Used to split string around spaces. 
	string arrival = "", departure = "";

	string word = "";
	ss >> word;				// Read a word 		
	x = CalculateMinutes(word);

	word = "";
	ss >> word;				// Read a word 		
	y = CalculateMinutes(word);
}

int main(int argc, char * argv[]) {
	
	try {
		// Проверяем начальные аргументы 
		if (argc != 2) {
			cerr << "Initial parameters missing...\n";
			throw 0;
		}

		int arr[9999], dep[9999], iV = 0;
		for (int i = 0; i < 9998; i++) {
			arr[i] = INT_MAX;
			dep[i] = INT_MAX;
		}

		MostVisitedInterval MG[9999] = {};

		ifstream file(argv[1]);
		if (file.is_open()) {
			string line;
			while (getline(file, line)) {
				ParseString(line, arr[iV], dep[iV]);
				iV++;
			}
			file.close();
		}
		else {
			cout << "Error during file IO\n";
			throw 0; // terminate with error
		}

		int iGT = DetermineMostVisitedInterval(arr, dep, iV, MG);

		for (int i = 0; i < iGT; i++) {
			printf("%d:%02d %d:%02d\n", (int)(MG[i].a / 60), MG[i].a % 60, (int)(MG[i].d / 60), MG[i].d % 60);
		}
	}
	catch (...) {		
		cerr << "Application ended with exeption (1)\n";
		//cin.get();
		return 1;
	}
	return 0;
}
