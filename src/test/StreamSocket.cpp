#include "StreamSocket.hpp"

StreamSocket::StreamSocket(const char *host, uint16_t port)
: _sFD(-1)
{
	// inet_addr resolve adress string to binary representation
	// of address in net byte order
	_host = inet_addr(host);

	// htons return network byte order format
	_port = htons(port);

	// open a stream socket(TCP) on IPv4 domain
	_sFD = socket(AF_INET, SOCK_STREAM, 0);
}

StreamSocket::~StreamSocket()
{
	close(_sFD); // close listening socket;
}

/*
** bind(), link socket with a host ip adress and port so it's accessable for
** other hosts to connect to.
*/
int StreamSocket::SockBind() const
{
	struct sockaddr_in hostAddr; // struct to store address and port to bind socket

	memset(&hostAddr, 0, sizeof(hostAddr)); // set it to 0;
	hostAddr.sin_family = AF_INET; // AF_INET = IPv4
	hostAddr.sin_port = _port;
	hostAddr.sin_addr.s_addr = _host;

	return bind(_sFD, (struct sockaddr *)&hostAddr, sizeof(hostAddr));
}

/*
**	listen(), set set socket listening and ready to recieve connextions
** if failed return -1, otherwise 0.
*/
int StreamSocket::SockListen() const
{
	return listen(_sFD, SOMAXCONN);
}

/*
**	accept method accept a new connection and return fd to change data with
** data with client
** if failed return -1
*/
SOCKET StreamSocket::SockAccept() const
{
	SOCKET clientFD;
	clientFD = accept(_sFD, NULL, NULL);
	return clientFD;
}

/*
** getter to get socket filedis usable for checking for errors and check if
** fd have a connection to accept.
** it returns only a copy so we can't miss with our file disc
** PS: don't close it's closed in the distructor.
*/
SOCKET StreamSocket::getFD() const
{
	return _sFD;
}
