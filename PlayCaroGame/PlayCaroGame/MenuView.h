#pragma once
#include "win.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "ChessTable.h"

using namespace std;

class MenuView
{
public:
	MenuView();
	~MenuView();
	static int PrintMenuLoginOrRegister();
	static void PrintMenuLogin(string &user,string &pass);
	static void PrintMenuRegister(string& user, string& pass);
	static int PrintMenuPlay();
	static void PrintInfoPlayer(string name, int win, int lose, int draw);
	static string PrintMenuChangePass(string user);
	static void EnterXYTable(int& row, int& col);
	static void Game_Record();
	static void PrintTurn(int turn) {
		if (turn == 1)
		{
			Win::gotoXY(0, 0);
			std::cout << "                 **********" << endl;
			std::cout << "                 * You: X *" << endl;
			std::cout << "                 **********" << endl;
		}
		else if (turn == 0)
		{
			Win::gotoXY(0, 0);
			std::cout << "                 **********" << endl;
			std::cout << "                 * You: O *" << endl;
			std::cout << "                 **********" << endl;
		}
	}
private:

};