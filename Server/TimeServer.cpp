#include "StreamSocket.hpp"
#include <time.h>
#include <sys/select.h>
#include <vector>

#define response  "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/plain\r\n\r\nLocal time is: 10:20"

int main()
{
	// set up servers
	char host[100] = "127.0.0.1";
	StreamSocket sockTest(host, 9090);

	if (sockTest.SockBind() < 0)
	{
		std::cout << "Bind error\n";
		return -1;
	}

	if (sockTest.SockListen() < 0)
	{
		std::cout << "Listen error\n";
		return -1;
	}

	std::vector<SOCKET> masterVec;
	masterVec.push_back(sockTest.getFD());
	int fdMax = sockTest.getFD();

	fd_set master;
	FD_ZERO(&master);
	FD_ZERO(&master);
	FD_SET(sockTest.getFD(), &master);
	// accept connextions, read requests and write responses
	std::cout << "check for connections... \n";
	while (1)
	{
		fd_set servTmp = master;
		select(fdMax + 1, &servTmp, 0, 0, 0);
		for (size_t i = 0; i < masterVec.size(); i++)
		{
			if (FD_ISSET(masterVec[i], &servTmp))
			{
				if (masterVec[i] == sockTest.getFD()) // accept new connextion
				{
					std::cout << " a connection to accept... \n";
					SOCKET cFD = sockTest.SockAccept();
					if (cFD < 0)
						std::cout << "accept error\n";
					FD_SET(cFD, &master);
					if (cFD > fdMax)
						fdMax = cFD;
					masterVec.push_back(cFD);
				}
				else
				{
					if (FD_ISSET(masterVec[i], &servTmp))
						std::cout << masterVec[i] << "is ready to read\n";
					// read request if it's complete get response, otherwise continue
					char buff[100];
					int readBuff = recv(masterVec[i], buff, 100, 0);
					buff[readBuff] = 0x00;
					std::cout << "bytes read = " << readBuff << " :" << buff << '\n';
					exit(0);
					// int sendBuff = send(masterVec[i], response, 100, 0);
					// std::cout << "bytes writen = " << sendBuff << "\n";
				}
			}
		}
	}
	return 0;
}
