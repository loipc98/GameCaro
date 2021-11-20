#pragma once

#include "MenuView.h"
#include "Player.h"
#include "ChessTable.h"
#include "Internet.h"

using namespace std;

class MenuControl
{
public:
	//Menu
	MenuControl(ChessTable& c, Player& pl);
	void ControlMenuLogin();
	void ControlMenuPlay();
	void RunGame();

	// Cac chuc nang
	void PlayGameOff();
	void PlayGameOnline();
	void xulyChuoi(int& x, int& y, string s);
	bool Login();
	bool Register();
	bool UpdateInfo();
	bool getInfor();
	bool ChangePass();//doi mat khau

	void updateChessTable();
	void updateSocketChessTable(int turn);
	double getTime(clock_t start);
private:
	ChessTable cTable;
	Player pPlayer;
};
