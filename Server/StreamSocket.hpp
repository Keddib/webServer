#ifndef STREAM_SOCKET_2021_HPP
#define STREAM_SOCKET_2021_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <errno.h>
#include <iostream>

#define SOCKET int // for readabality

/*
	StreamSocket object create a Stream socket and bind it with the addr
	and port given in constructor.
	if non of arguments is specifies it bind the socket to wildcard adress and
	default port 8080
*/

class StreamSocket
{
	private:
		SOCKET _sFD; // socket file discriptor
		uint32_t _host;
		uint16_t _port;
		StreamSocket( const StreamSocket & );
		StreamSocket& operator=(const StreamSocket &);
	public:
		StreamSocket(const char *host = "0.0.0.0", uint16_t port = 8080);
		// when host/port not given set default values
		~StreamSocket();
		int SockBind() const;
		int SockListen() const;
		SOCKET SockAccept() const;
		SOCKET getFD() const;
};

/*
	1.0 communication domains see (see sock domains in TCP/IP folder)
1.1 - UNIX (AF_UNIX) allow commun between apps on same host
1.2 - IPv4 (AF_INET) allow commun between apps on different hosts connected
	via an Internet Protocol version 4 (IPv4) network.
1.3 - IPv4 (AF_INET6) allow commun between apps on different hosts connected
	via an Internet Protocol version 6 (IPv6) network.
*/

/*
	2.0 socket types
2.1	Stream  sockets (SOCK_STREAM), connection-oriented, reliable, bidirectional, byte-stream
	TCP.
2.2 Datagram  sockets  ( SOCK_DGRAM), not reliable, connectionless (messages boundaries)
2.3 others... (SOCK_RAW) -> IP layer
*/


//struct in_addr {					/* IPv4 4-byte address */
//	in_addr_t s_addr;				/* Unsigned 32-bit integer */
//};
//
//struct sockaddr_in {				/* IPv4 socket address */
//	sa_family_t    sin_family;		/* Address family (AF_INET) */
//	in_port_t      sin_port;		/* Port number */ 16bit
//	struct in_addr sin_addr;		/* IPv4 address */ 32bit
//	unsigned char  __pad[X];		/* Pad to size of 'sockaddr'
//										structure (16 bytes) */
//};

/*
	2.0 sockaddr generic struct
each socket domain uses a different adress format (see sock domains in
TCP/IP folder), each sock domain has a different structure type.
sys calls like bind() are generic and in other to use all domain types
they accept a generic addrr structure (sockaddr). the only purpose for
this type is to cast the various domain-specific addrr structs to a single
type for use as argument in the sock sys-calls
struct sockaddr_in test;
struct sockaddr test;
*/

#endif
