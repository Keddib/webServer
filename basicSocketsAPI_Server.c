// #include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


int main()
{
	struct sockaddr_in local;
	int sfd;
	int s1;
	int rc;
	char buff2[1024];

	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(8000);
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
		if (rc <= 0)
		{
			perror("recv");
			return 1;
		}
		printf("%s\n", buff2);
		rc = send(s1, buff2, 1024, 0);
		if (rc <= 0)
			perror("send");
	}
		return 0;
}
