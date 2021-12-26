#include <time.h>
#include "./socketLib/inetSocket.hpp"

int main()
{
	std::cout << "opening socket... \n";
	SOCKET listenSocket = create_socket((char *)"localhost", 8080);
	if (listenSocket < 0)
	{
		std::cout << "listenSocket fialed\n";
		return -1;
	}

	std::cout << "listening on connections... \n";
	if (listen(listenSocket, 10) < 0)
	{
		std::cout << "listen() fialed\n";
		return -1;
	}
	std::cout << "accepting connections... \n";
	struct sockaddr_storage clientAddress;
	socklen_t sockAddressLen = sizeof(clientAddress);
	SOCKET clientSocket = accept(listenSocket,
						(struct sockaddr *)&clientAddress, &sockAddressLen);
	if (clientSocket < 0)
		std::cout << "accept failed... :(\n";
	std::cout << "client adress connected...\n";
	char clientBuffer[100];
	getnameinfo((struct sockaddr *)&clientAddress,
		sockAddressLen, clientBuffer, sizeof(clientBuffer),
		0, 0, NI_NUMERICHOST);
	std::cout << clientBuffer << "\n";

	std::cout << "reading request... \n";
	char request[1024];
	int bytesRead = recv(clientSocket, request, 1024, 0);
	if (bytesRead < 1024)
		request[bytesRead] = '\0';
	std::cout << "bytes read: " << bytesRead << "\n";
	std::cout << request;

	std::cout << "sending response... \n";
	const char *response = "HTTP/1.1 200 OK\r\n"
	"Connection: close\r\n"
	"Content-Type: text/plain\r\n\r\n"
	"Local time is: ";
	size_t responseSize = strlen(response);
	int byteSent = send(clientSocket ,response, responseSize, 0);
	std::cout << "Sent " << byteSent << " of " << responseSize << '\n';
	time_t timer;
	time(&timer);
	char *timeStr = ctime(&timer);
	responseSize = strlen(timeStr);
	byteSent = send(clientSocket, timeStr, responseSize, 0);
	std::cout << "Sent " << byteSent << " of " << responseSize << '\n';

	std::cout << "closing connection... \n";
	close(clientSocket);
	close(listenSocket);

	std::cout << "finished... \n";

	return 0;
}
