#include "Player.h"

void Player::setUser(string n) {
	user = n;
}

string Player::getUser() {
	return user;
}

void Player::setPass(string s) {
	password = s;
}

string Player::getPass() {
	return password;
}

void Player::setMatch(int win, int lose, int draw) {
	winMatch = win;
	loseMatch = lose;
	drawMatch = draw;
}

void Player::getMatch(int& win, int& lose, int& draw) {
	win = winMatch;
	lose = loseMatch;
	draw = drawMatch;
}