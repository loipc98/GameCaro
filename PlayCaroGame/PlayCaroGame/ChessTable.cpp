#include "ChessTable.h"

void ChessTable::setZeroArrTable() {
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			this->arrTable[i][j] = 0;
		}
	}
}

void ChessTable::printTable()
{
	Win::gotoXY(0, 3);
	Win::SetColor(0, 13);
	for (int i = 0; i <= 10; i++) {
		for (int j = 0; j <= 10; j++)
		{
			std::cout << std::setw(4) << std::right << "|";
		}
		std::cout << std::endl;
		std::cout << std::setfill('-');
		int c = 4 * (10 + 1);
		std::cout << std::setw(c) << "-" << std::endl;
		std::cout << std::setfill(' ');
	}

	for (int i = 0; i < 10; i++)
	{
		Win::goToPointBoard(i + 1, 0);
		std::cout << i;
	}
	for (int i = 0; i < 10; i++)
	{
		Win::goToPointBoard(0, i + 1);
		std::cout << i;
	}
}

void ChessTable::printPointTable(int row, int col, int value)
{
	Win::goToPointBoard(row, col);
	if (value == 1) std::cout << "X";
	else if (value == 2) std::cout << "O";
}

int ChessTable::getArrTable(int i, int j)
{
	return arrTable[i][j];
}

void ChessTable::setArrTable(int i, int j, int value)
{
	arrTable[i][j] = value;
}

int ChessTable::checkEndGame()
{
	int check = 0;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			//kiem tra theo cot
			if (arrTable[i][j] == 1 && arrTable[i + 1][j] == 1 && arrTable[i + 2][j] == 1 && arrTable[i + 3][j] == 1)
			{
				check = 1;
			}
			else if (arrTable[i][j] == 2 && arrTable[i + 1][j] == 2 && arrTable[i + 2][j] == 2 && arrTable[i + 3][j] == 2)
			{
				check = 2;
			}

			//kiem tra theo hang
			else if (arrTable[i][j] == 1 && arrTable[i][j + 1] == 1 && arrTable[i][j + 2] == 1 && arrTable[i][j + 3] == 1)
			{
				check = 1;
			}
			else if (arrTable[i][j] == 2 && arrTable[i][j + 1] == 2 && arrTable[i][j + 2] == 2 && arrTable[i][j + 3] == 2)
			{
				check = 2;
			}

			//kiem tra theo duong cheo
			else if (arrTable[i][j] == 1 && arrTable[i + 1][j + 1] == 1 && arrTable[i + 2][j + 2] == 1 && arrTable[i + 3][j + 3] == 1) {
				check = 1;
			}
			else if (arrTable[i][j] == 2 && arrTable[i + 1][j + 1] == 2 && arrTable[i + 2][j + 2] == 2 && arrTable[i + 3][j + 3] == 2) {
				check = 2;
			}
			else if (arrTable[i][9 - j] == 1 && arrTable[i + 1][9 - j - 1] == 1 && arrTable[i + 2][9 - j - 2] == 1 && arrTable[i + 3][9 - j - 3] == 1) {
				check = 1;
			}
			else if (arrTable[i][9 - j] == 2 && arrTable[i + 1][9 - j - 1] == 2 && arrTable[i + 2][9 - j - 2] == 2 && arrTable[i + 3][9 - j - 3] == 2) {
				check = 2;
			}
		}
	}
	return check;
}