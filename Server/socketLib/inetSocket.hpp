#ifndef INET_SOCKET_2021_HPP
#define INET_SOCKET_2021_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <fcntl.h>

fd_set test;

// FD_SETSIZE tes;
/*
The inetListen()  function  creates  a  listening  stream  (SOCK_STREAM)  socket  bound
to  the  wildcard  IP  address  on  the  TCP  port  specified  by  service.  This  function  is
designed for use by TCP servers.
*/

int inetListen(const char *host, const char *service, int backlog);
// Returns a file descriptor on success, or –1 on error
//  backlog pending connectionss for listen
// addrlen




// because Partial Reads and Writes on Stream Sockets may accure we need
// rewrite and reread until we reach the size that we want
ssize_t readn(int fd, void *buffer, size_t count);
//Returns number of bytes read, 0 on EOF, or –1 on error
ssize_t writen(int fd, void *buffer, size_t count);
//Returns number of bytes written, or –1 on error



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

// struct sockaddr_storage test;
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


// SOMAXCONN max of pending connections.

// int main()
// {
// 	// int i = SOMAXCONN;
// }

#endif
