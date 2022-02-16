#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
#include <iostream>


int main(int argc, char **argv)
{
	int port = std::stoi(argv[1]);
	struct sockaddr_in srv_addr;
	int srv_fd;
	char *REQ = "GET / HTTP/1.0\r\n\r\n";
	char RES[4096];

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(port);
	inet_aton("10.12.6.12", &srv_addr.sin_addr);

	srv_fd = socket(AF_INET, SOCK_STREAM, 0);
	std::cout << connect(srv_fd, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) << "\n";

	write (srv_fd, REQ, strlen(REQ));
	std::cout << "request sent\n";
	std::cout << "read?";
	int y;
	std::cin >> y;
	RES[read(srv_fd, RES, 4096)] = 0;

	std::cout << RES;

	close(srv_fd);
	

}
