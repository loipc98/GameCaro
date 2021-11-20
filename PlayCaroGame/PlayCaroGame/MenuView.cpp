#include "MenuView.h"


MenuView::MenuView()
{
}

MenuView::~MenuView()
{
}

int MenuView::PrintMenuLoginOrRegister()
{
	int select;
	system("cls");
	Win::SetColor(0, 7);
	cout << "==============================================" << endl;
	cout << "=    [1]: Dang nhap                          =" << endl;
	cout << "=    [2]: Dang ky                            =" << endl;
	cout << "=    [3]: Choi Offline                       =" << endl;
	cout << "=    [4]: Thoat                              =" << endl;
	cout << "==============================================" << endl;
	cout << "\nNhap lua chon cua ban: ";
NhapInputMenuLogin:
	cin >> select;
	while (std::cin.fail()) {
		std::cout << "Input Sai. Nhap lai lua chon cua ban: ";
		std::cin.clear();
		std::cin.ignore(256, '\n');   // ignore the line change
		std::cin >> select;
	}
	if (select < 0 || select>4) {
		std::cout << "Input Sai. Nhap lai lua chon cua ban:";
		goto NhapInputMenuLogin;
	}
	return select;
}

void MenuView::PrintMenuLogin(string& user, string& pass)
{
	system("cls");
	Win::SetColor(0, 7);
	cout << "=============================" << endl;
	cout << "=        Dang nhap          =" << endl;
	cout << "=============================" << endl;
	cout << "\nNhap tai khoan: ";
	cin >> user;
	cout << "Nhap mat khau: ";
	cin >> pass;
}

string MenuView::PrintMenuChangePass(string user) {
	system("cls");
	Win::SetColor(0, 7);
	cout << "=============================" << endl;
	cout << "=        Doi mat khau       =" << endl;
	cout << "=============================" << endl;
	string pass;
	cout << "\nTai khoan: " << user;
	cout << "\nNhap mat khau: ";
	cin >> pass;
	return pass;
}

void MenuView::PrintMenuRegister(string& user, string& pass)
{
	system("cls");
	Win::SetColor(0, 7);
	cout << "=============================" << endl;
	cout << "=        Dang nhap          =" << endl;
	cout << "=============================" << endl;
	cout << "\nNhap tai khoan: ";
	cin >> user;
	cout << "Nhap mat khau: ";
	cin >> pass;
}

int MenuView::PrintMenuPlay() {
	int select;
	system("cls");
	Win::SetColor(0, 7);
	cout << "==============================================" << endl;
	cout << "=    [1]: Choi Game                          =" << endl;
	cout << "=    [2]: Doi mat khau                       =" << endl;
	cout << "=    [3]: Xem thong tin                      =" << endl;
	cout << "=    [4]: Xem lai tran gan nhat              =" << endl;
	cout << "=    [5]: Thoat                              =" << endl;
	cout << "==============================================" << endl;
	cout << "\nNhap lua chon cua ban: ";
NhapInputMenuPlay:
	cin >> select;
	while (std::cin.fail()) {
		std::cout << "Input Sai. Nhap lai lua chon cua ban: ";
		std::cin.clear();
		std::cin.ignore(256, '\n');   // ignore the line change
		std::cin >> select;
	}
	if (select < 0 || select>4) {
		std::cout << "Input Sai. Nhap lai lua chon cua ban:";
		goto NhapInputMenuPlay;
	}
	return select;
}

void MenuView::PrintInfoPlayer(string user, int win, int lose, int draw) {
	system("cls");
	cout << "==============================" << endl;
	cout << "=    Thong tin nguoi choi    =" << endl;
	cout << "==============================" << endl;
	cout << "= User:                      =" << endl;
	cout << "= Win match:                 =" << endl;
	cout << "= Lose match:                =" << endl;
	cout << "= Draw match:                =" << endl;
	cout << "==============================" << endl;

	Win::gotoXY(15, 3);
	cout << user;
	Win::gotoXY(15, 4);
	cout << win;
	Win::gotoXY(15, 5);
	cout << lose;
	Win::gotoXY(15, 6);
	cout << draw;
	Win::gotoXY(0, 10);
	system("pause");
}

void MenuView::Game_Record() {
	system("cls");
	ChessTable::printTable();
	fstream fi;
	fi.open("record.txt", ios::in);
	string line;
	clock_t start, time_run;
	double time_use = 0;
	start = clock();
	while (!fi.eof()) {
		getline(fi, line);
		int pos;
		string str;
		if ((pos = line.find("-")) != string::npos) {
			str = line.substr(0, pos);
			double time_step = stod(str);
			while (time_use < time_step) {
				time_run = clock();
				time_use = (time_run - start) * 1.0 / CLOCKS_PER_SEC;
			}
			if (line.size() > pos) {
				pos = pos + 1;
				line.erase(0, pos);
				if (line == "X") {
					PrintTurn(1);
				}
				else if (line == "O") {
					PrintTurn(0);
				}
				else if (line == "Win" | line == "Lose" | line == "Draw") {
					Win::gotoXY(0, 28);
					cout << "You " + line << endl;
				}
				else {
					if ((pos = line.find(":")) != string::npos) {
						str = line.substr(0, pos);
						pos = pos + 1;
						line.erase(0, pos);
						int row, col;
						if (str == "Turn1") {
							row = atoi(line.c_str());
							if ((pos = line.find("#")) != string::npos) {
								pos = pos + 1;
								line.erase(0, pos);
								col = atoi(line.c_str());
								ChessTable::printPointTable(row + 1, col + 1, 1);
							}
						}
						else if (str == "Turn2") {
							row = atoi(line.c_str());
							if ((pos = line.find("#")) != string::npos) {
								pos = pos + 1;
								line.erase(0, pos);
								col = atoi(line.c_str());
								ChessTable::printPointTable(row + 1, col + 1, 2);
							}
						}
					}
				}
			}
		}
	}
	fi.close();
	system("pause");
}

void MenuView::EnterXYTable(int& row, int& col) {
	Win::gotoXY(0, 28);
	std::cout << "Nhap nuoc di cua ban: ";
Nhapbuocdion1:
	std::cin >> row >> col;
	//check loi
	while (std::cin.fail()) {
		std::cout << "Nhap sai. Nhap lai nuoc di cua ban: ";
		std::cin.clear();
		std::cin.ignore(256, '\n');   // ignore the line change
		goto Nhapbuocdion1;
	}
	if (row >= 10 || col >= 10 || row < 0 || col < 0) {
		std::cout << "Nhap sai. Nhap lai nuoc di cua ban: ";
		goto Nhapbuocdion1;
	}
}