#include "TcpClient.h"

TcpClient::TcpClient(int numberToSend, int flag): m_numberToSend(numberToSend), m_flag(flag)
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa))
		std::cout<< "WSAStartup failed: "<< WSAGetLastError();

	inet_pton(AF_INET, "88.212.241.115",&m_addr.sin_addr);
	m_addr.sin_port = htons(2013);
	m_addr.sin_family = AF_INET;

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket<=0)
		std::cout << "Socket failed: " << WSAGetLastError();
	
	if (!connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)))
		readWriteLoop();
}

void TcpClient::readWriteLoop()
{

	bool isEnd = false;
	std::string sendbuff;
	switch (m_flag)
	{
	case 0:
	{
		while (!isEnd)
		{
			sendbuff = std::to_string(m_numberToSend) + "\n";
			char buff[4]{};
			std::cout << "try send " << sendbuff << '\n';
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (send(m_socket, sendbuff.c_str(), sendbuff.length(), 0) < 0)
			{
				this->restart();
			}
			sendbuff.clear();
			int bytes_recv;
			bool digitEnds = false;
			while ((bytes_recv = recv(m_socket, buff, sizeof(buff), 0)) &&
				bytes_recv != SOCKET_ERROR)
			{
				if (this->isdigit(buff[0]))
					sendbuff.push_back(buff[0]);
				//else if (!sendbuff.empty())
				if (bytes_recv == 4)
					digitEnds = true;
				if (digitEnds)
					isEnd = true;
			}

			if (bytes_recv <= 0 && !isEnd)
			{
				//std::cout << '\n' << bytes_recv << " SOCKET_ERROR \n";
				this->restart();
			}
		}
		std::cout << "On number " << m_numberToSend << " get number " << sendbuff<< '\n';
		m_storage->addData(std::atoi(sendbuff.c_str()));
	} break;
	case 1:
	{
		sendbuff.append("Check ");
		sendbuff.append(std::to_string(m_numberToSend));
		sendbuff.append("\n");
		char buff[1024]{};
		std::cout << "try send " << sendbuff << '\n';
		if (send(m_socket, sendbuff.c_str(), sendbuff.length(), 0) < 0)
		{
			this->restart();
		}
		sendbuff.clear();
		int bytes_recv;
		while ((bytes_recv = recv(m_socket, buff, sizeof(buff), 0)) &&
			bytes_recv != SOCKET_ERROR)
		{
			std::cout << buff;
		}
	}
	}
}

int TcpClient::isdigit(int Char)
{
	if (Char >= 48 && Char<=57)
		return 1;
	else
		return 0;
}

void TcpClient::restart()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	WSACleanup();

	if (WSAStartup(MAKEWORD(2, 2), &m_wsa))
		std::cout << "WSAStartup failed: " << WSAGetLastError();

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_socket <= 0)
		std::cout << "Socket failed: " << WSAGetLastError();

	if (connect(m_socket, (SOCKADDR*)&m_addr, sizeof(m_addr)))
		std::cout << "Connection failed: " << WSAGetLastError();
}
