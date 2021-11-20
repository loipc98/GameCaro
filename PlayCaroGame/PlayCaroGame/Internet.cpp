#include "Internet.h"

int internet::SendData(string s)
{
	int rev = send(sock, s.c_str(), s.size() + 1, 0);
	return rev;
}

string internet::ReceiveData()
{
	char buf[1000];
	ZeroMemory(buf, 1000);
	int bytesReceived = recv(sock, buf, 1000, 0);
	string dataRev = "";
	if (bytesReceived > 0)
	{
		// Echo response to console
		dataRev = string(buf, 0, bytesReceived);
	}
	return dataRev;
}

//Connectting TCP/IP
bool internet::ConnectToServer()
{

	// Initialize WinSock
	WSAData data2;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data2);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return 0;
	}

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	else {
		return 1;
	}
}

//Disconnectting TCP/IP
void internet::disconnectting()
{
	closesocket(sock);
	WSACleanup();
}