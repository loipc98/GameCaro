#pragma once
#include <WS2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

static string ipAddress = "127.0.0.1";			// IP Address of the server
static int port = 54000;						// Listening port # on the server
static WSAData data2;
static WORD ver = MAKEWORD(2, 2);
static SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

class internet
{
public:
	//Hàm connectting to TCP/IP
	static bool ConnectToServer();

	//Hàm Disconnetting to TCP/IP
	static void disconnectting();

	static int SendData(string s);
	static string  ReceiveData();
};



