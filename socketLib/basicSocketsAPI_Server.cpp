// #include <sys/types.h>
#include "inetSocket.hpp"

#define PORT 8080

int main()
{
	struct sockaddr_in local;
	int sfd;
	int s1;
	int rc;
	char buff2[1024];
	char response[1024] = "HTTP/1.1 200 OK\nDate: Mon, 27 Jul 2009 12:28:53 GMT\nServer: Apache/2.2.14 (Win32)\nLast-Modified: Wed, 22 Jul 2009 19:15:56 GMT\nContent-Length: 88\nContent-Type: text/html\nConnection: Closed\n\n<html>\n<body>\n<h1>Hello, World!</h1>\n</body>\n</html>";

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(PORT);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
	{
		perror("Socket");
		return 1;
	}
	rc = bind(sfd, (struct sockaddr *)&local, sizeof(local));
	if (rc  < 0)
	{
		perror("bind");
		return 1;
	}
	rc = listen(sfd, 5);
	if (rc)
	{
		perror("listen");
		return 1;
	}
	while (1)
	{
		s1 = accept(sfd, NULL, NULL);
		if (s1 < 0)
		{
			perror("accept");
			return 1;
		}
		rc = recv(s1, buff2, 1024, 0);
		printf("rc from recv = %d\n", rc);
		if (rc <= 0)
		{
			perror("recv");
			return 1;
		}
		buff2[rc] = '\n';
		printf("%s\n", buff2);
		rc = send(s1, response, 1024, 0);
		if (rc <= 0)
			perror("send");
	}
		return 0;
}
