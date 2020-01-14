// task2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream> 
#include <algorithm>    // std::max
#include <fstream> // file io
#include <sstream>      // std::istringstream

using namespace std;

// Бесконечность
#define INF 99999 

// как проверить пересечение точки М с прямоугольником ABCD? 
// 1) от точки М проводим ЛУЧ на бесконечность вправо
// 2) считаем сколько раз луч из точки М пересечется с гранями прямоугольника ABCD
// 3) если имеется ОДНО пересечение с гранью прямоугольника ABCD то точка М лежит внутри
//      если имеется ДВА пересечения с гранью прямоугольника ABCD, то точка М лежит вне
//      если НЕТ пересечений с гранью прямоугольника ABCD, то точка М лежит вне
// 4) частные случаи в пункте (3)
// 4.1 точка М и вершина прямоугольника ABCD совпадают
// 4.2 если грань прямоугольника AB и точка М коллинеарны (лежат на одной прямой), то проверяем принадлежит ли точка М грани AB прямоугольника ABCD, иначе она не лежит внутри прямоугольника и не принадлежит грани AB

// координата точки
struct Point {
	float x;
	float y;
};

// 4.2 если грань прямоугольника AB и точка М коллинеарны (лежат на одной прямой), то проверяем принадлежит ли точка М грани AB прямоугольника ABCD, иначе она не лежит внутри прямоугольника и не принадлежит грани AB
// уже установлено что точки p, q, r коллинеарны (лежат на одной прямой), теперь покажем, что точка q принадлежит отрезку 'pr', иначе она находится вне грани прямоугольника
bool DoesPointBelongToFace(Point p, Point q, Point r) {
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

// определяем относительное положение грани прямоугольника АБ и точки М, которые образуют триплет точек (p, q, r).
// Случай 0 - три точки p, q и r коллинеарны (лежат на одной прямой)
// Случай 1 - три точки p, q и r направлены по часовой стрелке Clockwise
// Случай 2 - три точки p, q и r направлены против часовой стрелки Counterclockwise
int PointsOrientation(Point p, Point q, Point r) {
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // Три точки коллинеарны 
	return (val > 0) ? 1 : 2; // Точки направлены по или против часовой стрелки
}

// Проверяем пересечение грани прямоугольника AB (p1,q1) и луча из точки М (p2) направленного до бесконечности вправо (q2)
// Аксиома геометрии
// Возвращаем true если прямые 'p1q1' и 'p2q2' пересекаются
bool DoesLinesIntersect(Point p1, Point q1, Point p2, Point q2)
{
	// Проверка пересечения прямых AB и луча из точки М направленного на бесконечность вправо
	// Для этого проверяем ориентацию четырех точек относительно друг друга
	int o1 = PointsOrientation(p1, q1, p2);
	int o2 = PointsOrientation(p1, q1, q2);
	int o3 = PointsOrientation(p2, q2, p1);
	int o4 = PointsOrientation(p2, q2, q1);

	// Общая аксиома 
	if (o1 != o2 && o3 != o4)
		return true;

	// Частный случай аксиомы 
	// Три точки p1, q1 и p2 коллинеарны (лежат на одной прямой) и p2 лежит на прямой p1q1 
	if (o1 == 0 && DoesPointBelongToFace(p1, p2, q1)) return true;

	// Три точки p1, q1 и p2 коллинеарны (лежат на одной прямой) и q2 лежит на прямой p1q1 
	if (o2 == 0 && DoesPointBelongToFace(p1, q2, q1)) return true;

	// Три точки p2, q2 и p1 коллинеарны (лежат на одной прямой) и p1 лежит на прямой p2q2 
	if (o3 == 0 && DoesPointBelongToFace(p2, p1, q2)) return true;

	// Три точки p2, q2 и q1 коллинеарны (лежат на одной прямой) и q1 лежит на прямой p2q2 
	if (o4 == 0 && DoesPointBelongToFace(p2, q1, q2)) return true;

	return false; // Прямые НЕ пересекаются, не выполнился ни один из случаев 
}

// Определяет положение точки М (p) относительно прямоугольника ABCD (polygon[])
// 0 - точка на одной из вершин прямоугольника
// 1 - точка на одной из сторон прямоугольника
// 2 - точка внутри прямоугольника
// 3 - точка снаружи прямоугольника
int DoesPointBelongToRectangle(Point polygon[], Point p)
{
	// (1) Проверим совпадает ли точка М с одной из вершин многоугольника ABCD, тогда проверка закончена
	for (int i = 0; i < 4; i++) {
		if (polygon[i].x == p.x && polygon[i].y == p.y) return 0;
	}

	// (2) Точка М не совпадаем с вершиной прямоугольника, тогда проверяем находится ли точка внутри или лежит на грани
	// Провести луч из точки М (p) на бесконечность вправо 
	Point extreme = { INF, p.y };

	// NumIntersect - количество пересечений луча М с гранями прямоугольника ABCD
	int NumIntersect = 0, iVtx = 0;
	do
	{
		int nVtx = (iVtx + 1) % 4; // обходим вершины прямоугольинка

		//cout << std::boolalpha;
		//cout << "iVtx = " << iVtx << "; nVtx = " << nVtx << "; Nintsc = " << NumIntersect
		//	<< "; DLI = " << DoesLinesIntersect(polygon[iVtx], polygon[nVtx], p, extreme)
		//	<< "; PO = " << PointsOrientation(polygon[iVtx], p, polygon[nVtx])
		//	<< "; DBTF = " << DoesPointBelongToFace(polygon[iVtx], p, polygon[nVtx])
		//	<< " || R[" << polygon[iVtx].x << "," << polygon[iVtx].y << "], pt[" << p.x << "," << p.y << "]" << endl;

		// Проверяем пересечение грани прямоугольника AB (polygon[i],polygon[next]) и луча из точки М (p) направленного до бесконечности вправо (extreme)
		// Аксиома геометрии
		// Возвращаем true если прямые пересекаются
		if (DoesLinesIntersect(polygon[iVtx], polygon[nVtx], p, extreme))
		{
			// Проверяем коллинеарность прямой AB и точки М, в таком случае проверяем принадлежность точки М прямой AB
			// В ином случае точка М лежит вне прямой и не принадлежит прямоугольнику ABCD
			if (PointsOrientation(polygon[iVtx], p, polygon[nVtx]) == 0) {
				if (DoesPointBelongToFace(polygon[iVtx], p, polygon[nVtx])) return 1;	// точка М принадлежит грани многоугольника
				else return 3; // точка М на коллинеарной прямой с гранью прямоугольника ABCD, но лежит вне прямоугольника
			}

			NumIntersect++;
		}
		iVtx = nVtx;
	} while (iVtx != 0);

	if (NumIntersect == 1) return 2;	// одно пересечение луча из точки М с гранью, точка внутри прямоугольника
	else return 3;	// ни одного пересечения или более одного, точка вне прямоугольника
}

// Извлекаем значения из строки сохраненной в файле
void ParseString1(string str, float& x, float& y) {
	string word = "";
	for (auto i : str) {
		if (i == ' ') {
			x = stof(word);
			word = "";
		}
		else word = word + i;
	}
	y = stof(word);
}

// Извлекаем значения из строки сохраненной в файле
void ParseString2(string str, float& x, float& y) {
	istringstream ss(str);		// Used to split string around spaces. 

	string word = "";
	ss >> word;				// Read a word 		
	x = stof(word);

	word = "";
	ss >> word;				// Read a word 		
	y = stof(word);
}

int main(int argc, char * argv[]) {

	try {
		// Проверяем начальные аргументы 
		if (argc != 3) {
			cerr << "Initial parameters missing...\n";
			throw 0;
		}

		Point polygon[4] = {}, p[100] = {};
		int iR = 0, iP = 0;

		ifstream fileR(argv[1]), fileP(argv[2]);
		if (fileR.is_open() && fileP.is_open()) {
			string lineRect, linePoint;
			while (getline(fileR, lineRect)) {
				ParseString2(lineRect, polygon[iR].x, polygon[iR].y);
				iR++;
			}
			while (getline(fileP, linePoint)) {
				ParseString2(linePoint, p[iP].x, p[iP].y);
				iP++;
			}
			fileR.close();
			fileP.close();
		}
		else {
			cout << "Error during file IO\n";
			throw 0; // terminate with error
		}

		for (int i = 0; i < iP; i++) {
			cout << DoesPointBelongToRectangle(polygon, p[i]) << ",  p[" << p[i].x << "," << p[i].y << "]\n";
		}
	}
	catch (...) {
		cerr << "Application ended with exeption (1)\n";
		//cin.get();
		return 1;
	}
return 0;
}
