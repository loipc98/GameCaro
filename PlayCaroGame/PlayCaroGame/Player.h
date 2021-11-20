#pragma once
#include <string>

using namespace std;
class Player
{
public:
	Player() {
		user = "";
		password = "";
		winMatch = 0;
		loseMatch = 0;
		drawMatch = 0;
	}
	void setUser(string n);
	string getUser();
	void setPass(string s);
	string getPass();
	void setMatch(int win, int lose, int draw);
	void getMatch(int& win, int& lose, int& draw);
private:
	int match;
	string user;
	string password;
	int winMatch;
	int loseMatch;
	int drawMatch;
};

