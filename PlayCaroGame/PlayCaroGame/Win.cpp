#include "win.h"

void Win::goToPointBoard(int x, int y) {
	gotoXY(1 + 4 * x, 3 + 2 * y);
}

void Win::SetColor(int background_color, int text_color) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	int color_code = background_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

void Win::gotoXY(SHORT posx, SHORT posy) {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD Position;
	Position.X = posx;
	Position.Y = posy;

	SetConsoleCursorPosition(hStdout, Position);
}