#pragma once

#include <WS2tcpip.h>
#include "SqlDatabase.h"

#pragma comment (lib, "ws2_32.lib")


/*-----------Class TCP Server---------------------------------*/
class TCPServer :public SqlDatabase
{
private:
	SOCKET sock;
	SOCKET listening;
	//init tcpserver
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	// Create the master file descriptor set and zero it
	fd_set master;

	int check_play;
public:
	TCPServer();
	~TCPServer();
	void closeServer();
	int connectServer();
	void communication();
	void runServer();

};
