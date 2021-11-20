#include "TCPServer.h"


TCPServer::TCPServer()
{
	check_play = 0;
	WORD ver = MAKEWORD(2, 2);
	FD_ZERO(&master);
}

TCPServer::~TCPServer()
{
	closeServer();
}

int TCPServer::connectServer() {
	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return 0;
	}

	// Create a socket
	listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return 0;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);
	return 1;
}

void TCPServer::communication() {
	FD_ZERO(&master);
	// Add our first socket that we're interested in interacting with; the listening socket!
	// It's important that this socket is added for our server or else we won't 'hear' incoming
	// connections 
	FD_SET(listening, &master);

	// this will be changed by the \quit command
	bool running = true;
	fd_set pairing = master;
	while (running)
	{
		// Make a copy of the master file descriptor set, this is SUPER important because
		// the call to select() is _DESTRUCTIVE_. The copy only contains the sockets that
		// are accepting inbound connection requests OR messages. 

		// SO MAKE A COPY OF THE MASTER LIST TO PASS INTO select() !!!
		fd_set copy = master;

		// See who's talking to us
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

		// Loop through all the current connections / potential connect
		for (int i = 0; i < socketCount; i++)
		{
			// Makes things easy for us doing this assignment
			SOCKET sock = copy.fd_array[i];

			// Is it an inbound communication?
			if (sock == listening)
			{
				// Accept a new connection
				SOCKET client = accept(listening, nullptr, nullptr);

				// Add the new connection to the list of connected clients
				FD_SET(client, &master);
			}
			else // It's an inbound message
			{
				char buf[4096];
				ZeroMemory(buf, 4096);

				// Receive message
				int bytesIn = recv(sock, buf, 4096, 0);
				if (bytesIn <= 0)
				{
					// Drop the client
					closesocket(sock);
					FD_CLR(sock, &master);

					for (int j = 0; j < pairing.fd_count; j++)
					{
						SOCKET outsock = pairing.fd_array[j];
						if (outsock == sock) {
							string sendData = "Out";
							if (j % 2 == 1 && j > 0) {
								if (j + 1 <= pairing.fd_count) {
									send(pairing.fd_array[j + 1], sendData.c_str(), sendData.size() + 1, 0);
									FD_CLR(pairing.fd_array[j + 1], &pairing);
								}
								FD_CLR(pairing.fd_array[j], &pairing);
							}
							else if (j % 2 == 0 && j > 0)
							{
								send(pairing.fd_array[j - 1], sendData.c_str(), sendData.size() + 1, 0);
								FD_CLR(pairing.fd_array[j + 1], &pairing);
								FD_CLR(pairing.fd_array[j], &pairing);
							}
						}
					}
				}
				else
				{
					// Check to see if it's a command. \quit kills the server
					if (buf[0] == '\\')
					{
						// Is the command quit? 
						string cmd = string(buf, bytesIn);
						if (cmd == "\\quit")
						{
							running = false;
							break;
						}

						// Unknown command
						continue;
					}
					string data = string(buf, 0, bytesIn);
					if (data == "start")
					{
						FD_SET(sock, &pairing);
						for (int i = 0; i < pairing.fd_count; i++)
						{
							if (i>0 && i % 2 == 1) {
								SOCKET outSock = pairing.fd_array[i];
								string feedback = "wait";
								if (outSock != listening && outSock == sock)
								{
									send(outSock, feedback.c_str(), sizeof(feedback) + 1, 0);
								}
							}
							else if (i > 0 && i % 2 == 0) {
								SOCKET outSock = pairing.fd_array[i];
								string feedback = "play:" + to_string(i);
								if (outSock != listening && outSock == sock)
								{
									send(outSock, feedback.c_str(), sizeof(feedback) + 1, 0);
									feedback = "play:" + to_string(i - 1);
									send(pairing.fd_array[i - 1], feedback.c_str(), sizeof(feedback) + 1, 0);
								}
							}
						}
						//if (check_play == 0)
						//{
						//	for (int i = 0; i < master.fd_count; i++)
						//	{
						//		SOCKET outSock = master.fd_array[i];
						//		string feedback = "wait";
						//		if (outSock != listening && outSock == sock)
						//		{
						//			send(outSock, feedback.c_str(), sizeof(feedback) + 1, 0);
						//			check_play = 1;
						//		}
						//	}
						//}
						//else if (check_play == 1)
						//{
						//	for (int i = 0; i < master.fd_count; i++)
						//	{
						//		SOCKET outSock = master.fd_array[i];
						//		string feedback = "play:" + to_string(i);
						//		if (outSock != listening && outSock == sock)
						//		{
						//			send(outSock, feedback.c_str(), sizeof(feedback) + 1, 0);
						//			if (i % 2 == 1 && i > 0) {
						//				feedback = "play:" + to_string(i + 1);
						//				send(master.fd_array[i + 1], feedback.c_str(), sizeof(feedback) + 1, 0);
						//				check_play = 0;
						//			}
						//			else if (i % 2 == 0 && i > 0) {
						//				feedback = "play:" + to_string(i - 1);
						//				send(master.fd_array[i - 1], feedback.c_str(), sizeof(feedback) + 1, 0);
						//				check_play = 0;
						//			}
						//		}
						//	}
						//}
					}
					else
					{
						size_t pos = 0;
						std::string mode;
						if ((pos = data.find(":")) != std::string::npos)
						{
							mode = data.substr(0, pos);
							if (mode == "Login")
							{
								data.erase(0, pos + 1);
								if ((pos = data.find(",")) != std::string::npos)
								{
									std::string name = data.substr(0, pos);

									data.erase(0, pos + 1);
									std::string pass = data;

									//check user, password in database
									string feedback;
									if (check_login(name, pass) == 1)
									{
										feedback = "OK";
									}
									else
									{
										feedback = "Fail";
									}

									send(sock, feedback.c_str(), feedback.size() + 1, 0);

								}
							}
							else if (mode == "Regis")
							{
								data.erase(0, pos + 1);
								if ((pos = data.find(",")) != std::string::npos)
								{
									std::string name = data.substr(0, pos);

									data.erase(0, pos + 1);
									std::string pass = data;

									//check user, password in database
									//update user, password.
									//check user, password in database
									string feedback;
									if (check_register(name, pass) == 1)
									{
										string insert_query1 = "INSERT INTO PLAYER (Player_User, Player_Pass) VALUE ('" + name + "','" + pass + "')";
										const char* q = insert_query1.c_str();

										int qstate = mysql_query(conn, q);

										if (!qstate)
										{
											feedback = "OK";
										}
										else
										{
											feedback = "Fail";
										}
									}
									else
									{
										feedback = "Fail";
									}

									send(sock, feedback.c_str(), feedback.size() + 1, 0);
								}
							}
							else if (mode == "Pass") {
								data.erase(0, pos + 1);
								if ((pos = data.find(",")) != std::string::npos)
								{
									std::string name = data.substr(0, pos);

									data.erase(0, pos + 1);
									std::string pass = data;

									//check user, password in database
									string feedback;
									if (check_pass(name, pass) == 1)
									{
										if (updatePassWord(name, pass) == 1) {
											feedback = "OK";
										}
										else {
											feedback = "Fail";
										}
									}

									else
									{
										feedback = "Fail";
									}
									send(sock, feedback.c_str(), feedback.size() + 1, 0);
								}
							}
							else if (mode == "Update") {
								data.erase(0, pos + 1);
								string feedback = "Fail";
								if ((pos = data.find(":")) != std::string::npos) {
									string user;
									int win, lose, draw;
									user = data.substr(0, pos);
									data.erase(0, pos + 1);
									win = atoi(data.c_str());

									string cutStr = strchr(data.c_str(), ',');
									cutStr.erase(cutStr.begin());
									lose = atoi(cutStr.c_str());

									string cutStr1 = strchr(cutStr.c_str(), ',');
									cutStr1.erase(cutStr1.begin());
									draw = atoi(cutStr1.c_str());

									if (updateInforData(user, win, lose, draw) == 1) {
										feedback = "OK";
									}
								}
								send(sock, feedback.c_str(), feedback.size() + 1, 0);
								// send update data to server

							}
							else if (mode == "Infor") {
								data.erase(0, pos + 1);
								std::string name = data;
								string win = "";
								string lose = "";
								string draw = "";
								string feedback;
								if (check_Infor(name, win, lose, draw) == 1) {
									feedback = win + "," + lose + "," + draw;
									send(sock, feedback.c_str(), feedback.size() + 1, 0);
								}
								else {
									feedback = "Fail";
									send(sock, feedback.c_str(), feedback.size() + 1, 0);
								}
							}
							else if (mode == "Turn")
							{
								for (int i = 0; i < pairing.fd_count; i++)
								{

									SOCKET outSock = pairing.fd_array[i];
									if (outSock == sock)
									{
										ostringstream ss;
										ss << sock << ":" << buf << "\r\n";
										string strOut = ss.str();

										if (i % 2 == 1) send(pairing.fd_array[i + 1], strOut.c_str(), sizeof(strOut) + 1, 0);
										else if (i > 0 && i % 2 == 0) send(pairing.fd_array[i - 1], strOut.c_str(), sizeof(strOut) + 1, 0);
									}
								}
							}
						}
					}
				}
			}
		}

		//
		//for (int i = 1; i < pairing.fd_count; i = i + 2)
		//{
		//	// Makes things easy for us doing this assignment
		//	SOCKET sock1 = pairing.fd_array[i];
		//	SOCKET sock2 = pairing.fd_array[i + 1];
		//	// Is it an inbound communication?
		//	if (sock1 != listening && sock2 != listening)
		//	{
		//		char buf[4096];
		//		ZeroMemory(buf, 4096);
		//		// Receive message
		//		int bytesIn1 = recv(sock1, buf, 4096, 0);
		//		int bytesIn2 = recv(sock2, buf, 4096, 0);
		//		if (bytesIn1 <= 0 && bytesIn2 > 0)
		//		{
		//			string dataSend = "Out";
		//			send(sock2, dataSend.c_str(), dataSend.size() + 1, 0);
		//		}
		//		else if (bytesIn2 <= 0 && bytesIn1 > 0)
		//		{
		//			string dataSend = "Out";
		//			send(sock1, dataSend.c_str(), dataSend.size() + 1, 0);
		//		}
		//		else if(bytesIn1 <= 0 && bytesIn2 <= 0)
		//		{
		//			// Drop the client
		//			FD_CLR(sock1, &pairing);
		//			FD_CLR(sock2, &pairing);
		//		}
		//	}
		//}
	}
}

void TCPServer::closeServer() {
	/*------------Shutting server------------------------*/
	FD_CLR(listening, &master);
	closesocket(listening);

	// Message to let users know what's happening.
	string msg = "Server is shutting down. Goodbye\r\n";

	while (master.fd_count > 0)
	{
		// Get the socket number
		SOCKET sock = master.fd_array[0];

		// Send the goodbye message
		send(sock, msg.c_str(), msg.size() + 1, 0);

		// Remove it from the master file list and close the socket
		FD_CLR(sock, &master);
		closesocket(sock);
	}

	// Cleanup winsock
	WSACleanup();
}

void TCPServer::runServer() {
	if (ConnectToDatabase() == 1) {
		if (connectServer() == 1)
		{
			communication();
			closeServer();
		}
	}
}