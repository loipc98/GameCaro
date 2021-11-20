#include "MenuControl.h"

MenuControl::MenuControl(ChessTable& c, Player& pl)
{
	cTable = c;
	pPlayer = pl;
}

void MenuControl::ControlMenuLogin() {
	int slogin;
MenuLoginOrRegister:
	slogin = MenuView::PrintMenuLoginOrRegister();
	switch (slogin)
	{
	case 1:
		if (internet::ConnectToServer() == 1)
		{
			if (Login() == 0) {
				internet::disconnectting();
				goto MenuLoginOrRegister;
			}
			else
			{
				bool check = getInfor();
				if (check == 1)
				{
					int win, lose, draw;
					pPlayer.getMatch(win, lose, draw);
					internet::disconnectting();
					ControlMenuPlay();
				}
				else
				{
					internet::disconnectting();
					goto MenuLoginOrRegister;
				}
			}
		}
		else goto MenuLoginOrRegister;
		break;
	case 2:
		if (internet::ConnectToServer() == 1) {
			Register();
			internet::disconnectting();
		}
		else goto MenuLoginOrRegister;
		break;
	case 3:
		PlayGameOff();
		goto MenuLoginOrRegister;
		break;
	case 4:
		break;
	default:
		break;
	}

}

void MenuControl::ControlMenuPlay() {
	int inplay;
MenuPlay:
	inplay = MenuView::PrintMenuPlay();
	switch (inplay)
	{
	case 1:
		//choi game
		if (internet::ConnectToServer() == 1) {
			PlayGameOnline();
			internet::disconnectting();
		}
		goto MenuPlay;
		break;
	case 2:
		//doi mat khau
		if (internet::ConnectToServer() == 1) {
			ChangePass();
			internet::disconnectting();
		}
		goto MenuPlay;
		break;
	case 3:
		//Thong tin nguoi choi
		int win, lose, draw;
		pPlayer.getMatch(win, lose, draw);
		MenuView::PrintInfoPlayer(pPlayer.getUser(), win, lose, draw);
		goto MenuPlay;
		break;
	case 4:
		//Xem record game
		MenuView::Game_Record();
		goto MenuPlay;
		break;
	case 5:
		//thoat
		ControlMenuLogin();
		break;
	default:
		break;
	}
}

void MenuControl::PlayGameOff()
{
	system("cls");
	Win::SetColor(0, 12);
	cout << "Player1(X) ";
	cout << " - ";
	cout << "Player2(0) " << endl;
	cTable.setZeroArrTable();
	cTable.printTable();
	updateChessTable();
}

void MenuControl::updateChessTable() {
	int turnPlayer = 1;
	int step = 0;
	int checkGame = 0;
	int value = 1;
	int error = 0;
	while (step < 100 && cTable.checkEndGame() == 0)
	{
		Win::gotoXY(0, 28);
		std::cout << setw(100) << " " << endl;
		std::cout << setw(100) << " ";
		Win::gotoXY(0, 28);
		int row, col;
		cout << "Nhap nuoc di cua Player " << turnPlayer << " :";
	NhapNuocDiOff:
		cin >> row >> col;
		while (std::cin.fail()) {
			std::cout << "Nhap sai du lieu. Nhap lai nuoc cua ban:";
			std::cin.clear();
			std::cin.ignore(256, '\n');   // ignore the line change
			goto NhapNuocDiOff;
		}
		if (row >= 10 || col >= 10 || row < 0 || col < 0) {
			cout << "Nhap sai. Nhap lai nuoc di cua ban: ";
			goto NhapNuocDiOff;
		}
		if (row >= 0 && col >= 0 && row < 10 && col < 10) {
			if (cTable.getArrTable(row, col) != 0) {
				Win::gotoXY(0, 27);
				std::cout << "Nuoc di da duoc di truoc do. Vui long di lai: ";
				error = 1;
				continue;
			}
		}

		//Bo chu thich nhap lai
		if (error == 1) {
			Win::gotoXY(0, 27);
			std::cout << "                                                          ";
			error = 0;
		}

		cTable.printPointTable(col + 1, row + 1, value);
		cTable.setArrTable(row, col, value);
		step++;

		if (turnPlayer == 1)
		{
			value = 2;
			turnPlayer = 2;
		}
		else if (turnPlayer == 2) {
			value = 1;
			turnPlayer = 1;
		}
		Win::gotoXY(0, 28);
	}

	checkGame = cTable.checkEndGame();

	Win::gotoXY(0, 30);
	switch (checkGame)
	{
	case 0:
		std::cout << "Two player are draw!";
		break;
	case 1:
		std::cout << "Player 1 win!" << std::endl;
		break;
	case 2:
		std::cout << "Player 2 win!" << std::endl;
		break;
	default:
		break;
	}
}

void MenuControl::PlayGameOnline() {
	// Do-while loop to send and receive data
	char buf[1000];
	ZeroMemory(buf, 1000);
	int turn;
	string data = "";
	internet::SendData("start");
	cout << "Dang tim kiem nguoi choi...";
	data = internet::ReceiveData();
	//if receive "wait:turn", wait other player
	//else if receive "play:turn", play game
	if (data == "wait") {
		data = internet::ReceiveData();
	}
	//Play
	size_t pos = 0;
	if ((pos = data.find(":")) != std::string::npos) {
		data.erase(0, pos + 1);
		turn = atoi(data.c_str());
	}
	turn = turn % 2;

	system("cls");
	cTable.setZeroArrTable();
	cTable.printTable();
	//cap nhat buoc di qua server (sử dụng vòng lặp while đk kết thúc)
	updateSocketChessTable(turn);
	//in thông tin người thắng
}

double MenuControl::getTime(clock_t start) {
	clock_t end = clock();
	clock_t t = end - start;
	double time_use = (double)(t * 1.0 / CLOCKS_PER_SEC);
	return time_use;
}

void MenuControl::updateSocketChessTable(int turn)
{
	int step = 0;
	int value;
	int check_end = 0;
	string data_record;
	clock_t start_time;
	double time_run = 0;
	start_time = clock();
	if (turn == 1)
	{
		time_run = getTime(start_time);
		data_record = to_string(time_run) + "-X";
	}
	else if (turn == 0)
	{
		time_run = getTime(start_time);
		data_record = to_string(time_run) + "-O";
	}
	do
	{
		if (turn == 1)
		{
			MenuView::PrintTurn(turn);

			//Nhap buoc di tren ban co
			value = 1;
			Win::gotoXY(0, 27);
			for (int i = 0; i < 5; i++)
			{
				std::cout << setw(100) << " " << endl;
			}

			int row, col;
		EnterStep1:
			MenuView::EnterXYTable(row, col);
			if (row >= 0 && col >= 0 && row < 10 && col < 10) {
				if (cTable.getArrTable(row, col) != 0) {
					std::cout << "Nuoc di da duoc di truoc do. Vui long di lai: ";
					goto EnterStep1;
				}
			}

			//Send data through socket
			string send = "Turn:" + to_string(row) + ";" + to_string(col);
			internet::SendData(send);

			//Kiem tra sendResult
			time_run = getTime(start_time);
			data_record += "\n" + to_string((time_run)) + "-Turn" + to_string(value) + ":" + to_string(row) + "#" + to_string(col);
			cTable.printPointTable(col + 1, row + 1, value);
			cTable.setArrTable(row, col, value);
			step++;

			if (cTable.checkEndGame() == 1) goto checkgame;

			//Doi nguoi khac nhap buoc di
			value = 2;
			Win::gotoXY(0, 27);
			for (int i = 0; i < 5; i++)
			{
				std::cout << setw(100) << " " << endl;
			}
			Win::gotoXY(0, 28);
			std::cout << "Doi buoc di cua nguoi choi khac...  ";
			//wait data from socket server and cout step
			string dataRev = internet::ReceiveData();
			if (dataRev == "Out") {
				Win::gotoXY(0, 27);
				for (int i = 0; i < 5; i++)
				{
					std::cout << setw(100) << " " << endl;
				}
				Win::gotoXY(0, 27);
				std::cout << "Nguoi choi da thoat.";
				check_end = 1;
				goto OutGame;
			}
			else if (dataRev != "")
			{
				xulyChuoi(row, col, dataRev);
				std::cout << row << " " << col << endl;
				time_run = getTime(start_time);
				data_record += "\n" + to_string(time_run) + "-Turn" + to_string(value) + ":" + to_string(row) + "#" + to_string(col);
				cTable.printPointTable(col + 1, row + 1, value);
				cTable.setArrTable(row, col, value);
				step++;
			}
		}

		else if (turn == 0)
		{
			//Doi nguoi khac
			value = 1;
			MenuView::PrintTurn(turn);
			Win::gotoXY(0, 27);
			for (int i = 0; i < 5; i++)
			{
				std::cout << setw(100) << " " << endl;
			}
			int row, col;
			Win::gotoXY(0, 28);
			std::cout << "Doi buoc di cua nguoi choi khac...  ";

			//wait data from socket server and cout step
			string dataRev = internet::ReceiveData();
			if (dataRev == "Out") {
				Win::gotoXY(0, 27);
				for (int i = 0; i < 5; i++)
				{
					std::cout << setw(100) << " " << endl;
				}
				Win::gotoXY(0, 27);
				std::cout << "Nguoi choi da thoat.";
				check_end = 2;
				goto OutGame;
			}
			if (dataRev != "")
			{
				xulyChuoi(row, col, dataRev);
				std::cout << row << " " << col << endl;
				time_run = getTime(start_time);
				data_record += "\n" + to_string(time_run) + "-Turn" + to_string(value) + ":" + to_string(row) + "#" + to_string(col);
				cTable.printPointTable(col + 1, row + 1, value);
				cTable.setArrTable(row, col, value);
				step++;
			}

			if (cTable.checkEndGame() == 1) goto checkgame;

			//Nhap buoc di tren ban co
			value = 2;
			Win::gotoXY(0, 27);
			for (int i = 0; i < 5; i++)
			{
				std::cout << setw(100) << " " << endl;
			}

		EnterStep2:
			MenuView::EnterXYTable(row, col);
			if (row >= 0 && col >= 0 && row < 10 && col < 10) {
				if (cTable.getArrTable(row, col) != 0) {
					std::cout << "Nuoc di da duoc di truoc do. Vui long di lai: ";
					goto EnterStep2;
				}
			}

			//Send data through socket
			string send = "Turn:" + to_string(row) + ";" + to_string(col);
			internet::SendData(send);
			//Kiem tra sendResult
			time_run = getTime(start_time);
			data_record += "\n" + to_string(time_run) + "-Turn" + to_string(value) + ":" + to_string(row) + "#" + to_string(col);
			cTable.printPointTable(col + 1, row + 1, value);
			cTable.setArrTable(row, col, value);
			step++;
		}
	checkgame:
		check_end = cTable.checkEndGame();
	} while (step < 100 && check_end == 0);

	Win::gotoXY(0, 27);
	for (int i = 0; i < 5; i++)
	{
		std::cout << setw(100) << " " << endl;
	}

OutGame:
	Win::gotoXY(0, 28);
	int win, lose, draw;
	pPlayer.getMatch(win, lose, draw);
	switch (check_end)
	{
	case 0:
		cout << "You are draw!";
		data_record += "\n" + to_string((getTime(start_time))) + "-Draw";
		pPlayer.setMatch(win, lose, draw + 1);
		UpdateInfo();
		system("pause");
		break;
	case 1:
		if (turn == 1) {
			std::cout << "You win!" << std::endl;
			pPlayer.setMatch(win + 1, lose, draw);
			data_record += "\n" + to_string((getTime(start_time))) + "-Win";
		}
		else {
			std::cout << "You lose!" << std::endl;
			data_record += "\n" + to_string((getTime(start_time))) + "-Lose";
		}
		UpdateInfo();
		system("pause");
		break;
	case 2:
		if (turn == 1) {
			std::cout << "You lose!" << std::endl;
			data_record += "\n" + to_string((getTime(start_time))) + "-Lose";
			pPlayer.setMatch(win, lose + 1, draw);
		}
		else {
			std::cout << "You win!" << std::endl;
			data_record += "\n" + to_string((getTime(start_time))) + "-Win";
			pPlayer.setMatch(win + 1, lose, draw);
		}
		UpdateInfo();
		system("pause");
		break;
	default:
		break;
	}
	//Luu thong tin tran vao file record
	fstream f;
	f.open("record.txt", ios::out);
	f << data_record;
	f.close();
}

void MenuControl::xulyChuoi(int& xTable, int& yTable, string str) {

	string cutStr = strchr(str.c_str(), ':');
	cutStr.erase(cutStr.begin());
	string cutStr1 = strchr(cutStr.c_str(), ':');
	cutStr1.erase(cutStr1.begin());
	xTable = atoi(cutStr1.c_str());

	string cutStr2 = strchr(str.c_str(), ';');
	cutStr2.erase(cutStr2.begin());
	yTable = atoi(cutStr2.c_str());

}

bool MenuControl::Login() {
	string user = "";
	string pass = "";
	MenuView::PrintMenuLogin(user, pass);
	//send data to server
	string data = "Login:" + user + "," + pass;
	internet::SendData(data);
	string dataRev = internet::ReceiveData();
	if (dataRev == "OK") {
		cout << "Dang nhap thanh cong!" << endl;
		system("pause");
		pPlayer.setUser(user);
		//Lay thong tin nguoi choi

		return 1;
	}
	else if (dataRev == "Fail")
	{
		cout << "Dang nhap that bai!" << endl;
		system("pause");
		return 0;
	}
	return 0;
}

bool MenuControl::Register() {
	string user = "";
	string pass = "";
	MenuView::PrintMenuLogin(user, pass);
	//send data to server
	string data = "Regis:" + user + "," + pass;
	internet::SendData(data);
	string dataRev = internet::ReceiveData();
	if (dataRev == "OK") {
		cout << "Dang ky thanh cong!" << endl;
		system("pause");
		return 1;
	}
	else if (dataRev == "Fail")
	{
		cout << "Dang ky khong thanh cong. Ten nguoi choi da duoc su dung" << endl;
		system("pause");
		return 0;
	}
	return 0;
}

bool MenuControl::ChangePass()
{
	//Lay ten
	string user = pPlayer.getUser();
	string pass = MenuView::PrintMenuChangePass(user);
	string data = "Pass:" + user + "," + pass;
	internet::SendData(data);
	string dataRev = internet::ReceiveData();
	if (dataRev == "OK") {
		cout << "Doi mat khau thanh cong!" << endl;
		pPlayer.setPass(pass);
		system("pause");
		return 1;
	}
	else if (dataRev == "Fail")
	{
		cout << "Khong the doi mat khau!" << endl;
		system("pause");
		return 0;
	}
	return 0;
}

bool MenuControl::getInfor() {
	internet::SendData("Infor:" + pPlayer.getUser());
	string dataRev = internet::ReceiveData();
	//Xu ly cat chuoi de lay ra tran win, lose, draw
	if (dataRev == "Fail") {
		Win::gotoXY(0, 8);
		cout << "Khong lay duoc du lieu tu Server!";
		system("pause");
		return 0;
	}
	else {
		size_t pos = 0;
		int win, lose, draw;
		win = atoi(dataRev.c_str());

		string cutStr = strchr(dataRev.c_str(), ',');
		cutStr.erase(cutStr.begin());
		lose = atoi(cutStr.c_str());

		string cutStr1 = strchr(cutStr.c_str(), ',');
		cutStr1.erase(cutStr1.begin());
		draw = atoi(cutStr1.c_str());

		pPlayer.setMatch(win, lose, draw);
		return 1;
	}
}

void MenuControl::RunGame() {
	ControlMenuLogin();
}

bool MenuControl::UpdateInfo() {
	int win, lose, draw;
	pPlayer.getMatch(win, lose, draw);
	string data = "Update:" + pPlayer.getUser() + ":" + to_string(win) + "," + to_string(lose) + "," + to_string(draw);
	internet::SendData(data);
	string dataRev = internet::ReceiveData();
	if (dataRev == "OK") return 1;
	else return 0;
}

