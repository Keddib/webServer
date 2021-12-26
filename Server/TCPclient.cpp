#include "socketLib/inetSocket.hpp"
#include <fstream>

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "usage: TCPclient hostname port\n";
		return -1;
	}
	std::cout << "configuring address... \n";
	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	struct addrinfo *peerAddress;
	if (getaddrinfo(argv[1], argv[2], &hints, &peerAddress))
	{
		std::cerr << "getaddrinfo failed \n";
		return -1;
	}
	std::cout << "Remote address is: \n";
	char addressBuff[100];
	char serviceBuff[100];
	getnameinfo(peerAddress->ai_addr, peerAddress->ai_addrlen,
			addressBuff, sizeof(addressBuff),
			serviceBuff, sizeof(serviceBuff),
			NI_NUMERICHOST);
	std::cout << addressBuff <<":"<< serviceBuff << "\n";
	// std::cout << peerAddress->ai_family << "\n";
	// std::cout << peerAddress->ai_addr << "\n";
	// std::cout << peerAddress->ai_socktype << "\n";
	// std::cout << peerAddress->ai_protocol << "\n";

	std::cout << "create socket... \n";
	SOCKET socketfd(-1);
	int rc(0);

	if ((socketfd = socket(peerAddress->ai_family,
		peerAddress->ai_socktype, 0)) < 0)
	{
		std::cerr << "socket failed...\n";
		return -1;
	}
	struct sockaddr_in peer;
	peer.sin_addr.s_addr = peerAddress->ai_addr->sa_len;
	peer.sin_family = peerAddress->ai_family;
	peer.sin_port = htons(8080);
	std::cout << "connecting to server...\n";
	if(connect(socketfd, (struct sockaddr*)&peer, sizeof(peer)))
	{
		std::cerr << "connect failes... \n";
		std::cout << errno;
		return -1;
	}
	freeaddrinfo(peerAddress); // free peer address
	peerAddress = NULL;
	std::cout << "connected :D\n to send data, enter text followed by enter\n";
	// multiplexing i/o
	fd_set mainFDs;
	FD_ZERO(&mainFDs);
	FD_SET(socketfd, &mainFDs);
	FD_SET(0, &mainFDs);
	while (1)
	{
		fd_set readyFDs = mainFDs;
		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 100000;
		if (select(socketfd + 1, &readyFDs, NULL, NULL, &timeout) < 0)
		{
			std::cerr << "select() failed... \n";
			return -1;
		}
		if (FD_ISSET(socketfd, &readyFDs))
		{
			char readBuff[4096];
			int bytesRecieved = recv(socketfd, readBuff, sizeof(readBuff), 0);
			readBuff[4095] = '\0';
			if (bytesRecieved < 1) {
				std::cerr << "connection closed by peer\n";
				break;
			}
			std::cout << "Bytes recieved = " << bytesRecieved << " : " << readBuff << "\n";
		}
		if (FD_ISSET(0, &readyFDs))
		{
			std::ifstream in(0, std::ios::binary);

			char readBuff[4096];
			in.read(readBuff, sizeof(readBuff));
			std::cout << "sending data... \n";
			int bytesSend = send(socketfd, readBuff, sizeof(readBuff), 0);
			std::cout << "bytes sent : " << bytesSend << '\n';
		}
	}
	return 0;
}
