#pragma once
#include <WS2tcpip.h>
class Win
{
public:
	Win();
	~Win();
	static void gotoXY(SHORT posx, SHORT posy);
	static void SetColor(int background_color, int text_color);
	static void goToPointBoard(int x, int y);
};
