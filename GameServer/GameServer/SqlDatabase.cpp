#include "SqlDatabase.h"

using namespace std;

SqlDatabase::SqlDatabase()
{
}

SqlDatabase::~SqlDatabase()
{
}

bool SqlDatabase::check_login(string user, string passwd)
{
	// Variable 
	int tt;
	//Chose account
	tt = mysql_query(conn, ("SELECT * FROM Player WHERE Player_User='" + user + "' and Player_Pass='" + passwd + "'").c_str());
	if (!tt)
	{
		res = mysql_store_result(conn);
		if (res != NULL)
		{
			if (mysql_num_rows(res) > 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		cout << "QUERY EXCUTION PROBLEM" << mysql_error(conn) << endl;
		return 0;
	}
}

bool SqlDatabase::check_pass(string user, string passwd) {
	// Variable 
	int tt;
	//Chose account
	tt = mysql_query(conn, ("SELECT * FROM Player WHERE Player_User='" + user + "' and Player_Pass='" + passwd + "'").c_str());
	if (!tt)
	{
		res = mysql_store_result(conn);
		if (res != NULL)
		{

			if (mysql_num_rows(res) == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		cout << "QUERY EXCUTION PROBLEM" << mysql_error(conn) << endl;
		return 0;
	}
}

bool SqlDatabase::check_register(string user, string passwd)
{
	// Variable 
	int tt;
	//Chose account
	tt = mysql_query(conn, ("SELECT * FROM Player WHERE Player_User='" + user + "'").c_str());
	if (!tt)
	{
		res = mysql_store_result(conn);
		if (res != NULL)
		{

			if (mysql_num_rows(res) > 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		cout << "QUERY EXCUTION PROBLEM" << mysql_error(conn) << endl;
		return 0;
	}
}

bool SqlDatabase::check_passWord(string user, string passwd) {
	// Variable 
	int tt;
	//Chose account
	tt = mysql_query(conn, ("SELECT * FROM Player WHERE Player_User='" + user + "' and Player_Pass='" + passwd + "'").c_str());
	if (!tt)
	{
		res = mysql_store_result(conn);
		if (res != NULL)
		{

			if (mysql_num_rows(res) == 0)	return 1;
			else	return 0;
		}
		else return 0;
	}
	else
	{
		cout << "QUERY EXCUTION PROBLEM" << mysql_error(conn) << endl;
		return 0;
	}
}

bool SqlDatabase::updatePassWord(string user, string pass) {
	string insert_query1 = "UPDATE PLAYER SET Player_Pass = '" + pass + "' WHERE Player_User ='" + user + "'";
	const char* q = insert_query1.c_str();

	int qstate = mysql_query(conn, q);

	if (!qstate)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool SqlDatabase::updateInforData(string user, int win, int lose, int draw) {
	string insert_query1 = "UPDATE player SET Win = '" + to_string(win) + "', Lose = '" + to_string(lose) + "', Draw = '" + to_string(draw) + "' WHERE Player_User = '" + user + "'";
	const char* q = insert_query1.c_str();

	int qstate = mysql_query(conn, q);

	if (!qstate)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool SqlDatabase::check_Infor(string user, string& win, string& lose, string& draw)
{
	int tt;
	//Chose account
	tt = mysql_query(conn, ("SELECT * FROM Player WHERE Player_User='" + user + "'").c_str());
	if (!tt)
	{
		res = mysql_store_result(conn);
		if (res != NULL)
		{
			while (row = mysql_fetch_row(res)) {
				win = row[3];
				lose = row[4];
				draw = row[5];
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return 0;
	}
}

int SqlDatabase::ConnectToDatabase()
{
	conn = mysql_init(0);
	if (conn)
	{
		cout << "Database Connect..." << endl;
	}
	else
	{
		cout << "Fail to Connect!" << mysql_error(conn) << endl;
	}
	conn = mysql_real_connect(conn, "localhost", "root", "", "userdata", 3306, NULL, 0);
	if (conn)
	{
		cout << "Database Connect To Mysql" << conn << endl;
		//			system("pause");
		return 1;
	}
	else
	{
		cout << "Fail To Connect!" << mysql_error(conn) << endl;
		return 0;
	}
}