#include <iostream>
#include "MenuControl.h"
#include <WS2tcpip.h>


using namespace std;

int main() {
	ChessTable cTable;
	Player pPlayer;
	MenuControl menu(cTable, pPlayer);
	menu.RunGame();
	return 0;
}