#pragma once
#include "win.h"
#include <iostream>
#include <iomanip>

class ChessTable
{
public:
	static void printTable();
	static void printPointTable(int row, int col, int value);
	int getArrTable(int i, int j);
	void setArrTable(int i, int j, int value);
	void setZeroArrTable();
	int checkEndGame();
private:
	int arrTable[10][10];
};
