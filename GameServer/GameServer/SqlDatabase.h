#pragma once
#include <mysql.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class SqlDatabase
{
public:
	SqlDatabase();
	~SqlDatabase();
	//Connect to database
	int ConnectToDatabase();

	//Xu ly du lieu nhan duoc va tuong tac voi server
	bool updatePassWord(string user, string pass);
	bool check_login(string user, string passwd);
	bool check_pass(string user, string passwd);
	bool check_register(string user, string passwd);
	bool check_passWord(string user, string passwd);
	bool updateInforData(string user, int win, int lose, int draw);
	bool check_Infor(string user, string& win, string& lose, string& draw);
protected:
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
};