#pragma once
#pragma comment (lib, "Ws2_32.lib")

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <cctype>
#include <thread>
#include <chrono>
#include "Storage.h"


class TcpClient
{
private:
	WSADATA m_wsa;
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	int m_numberToSend;
	int m_flag;
	Storage* m_storage = m_storage->getInstance();

public:
	TcpClient(int numberToSend, int flag);

private:
	void readWriteLoop();
	int isdigit(int Char);
	void restart();

};

