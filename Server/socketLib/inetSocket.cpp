#include "inetSocket.hpp"

int create_socket(char *addr, uint16_t port)
{
	int sfd(-1), rc(-1);
	struct sockaddr_in local;

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(port);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		return -1;
	rc = bind(sfd, (struct sockaddr *)&local, sizeof(local));
	if (rc  < 0)
		return -1;
}
