#include <string>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
/*
** defining macros for content types
*/

#define JPG "image/jpg"

const char *get_content_type(std::string path)
{
	const char *lastDot = std::strrchr(path.c_str(), '.');
	if (!lastDot)
		return NULL;
	if (std::strcmp(lastDot, ".css") == 0) return "text/css";
	if (std::strcmp(lastDot, ".csv") == 0) return "text/csv";
	if (std::strcmp(lastDot, ".gif") == 0) return "image/gif";
	if (std::strcmp(lastDot, ".htm") == 0) return "text/html";
	if (std::strcmp(lastDot, ".html") == 0) return "text/html";
	if (std::strcmp(lastDot, ".ico") == 0) return "image/x-icon";
	if (std::strcmp(lastDot, ".jpeg") == 0) return "image/jpeg";
	if (std::strcmp(lastDot, ".jpg") == 0) return "image/jpeg";
	if (std::strcmp(lastDot, ".js") == 0) return "application/javascript";
	if (std::strcmp(lastDot, ".json") == 0) return "application/json";
	if (std::strcmp(lastDot, ".png") == 0) return "image/png";
	if (std::strcmp(lastDot, ".pdf") == 0) return "application/pdf";
	if (std::strcmp(lastDot, ".svg") == 0) return "image/svg+xml";
	if (std::strcmp(lastDot, ".txt") == 0) return "text/plain";

	return "application/octet-stream";
}


int main()
{
	std::string str = get_content_type("test.svg");
	int i = write(1, str.c_str(), str.size());
	std::cout << "\ni : " << i;
}
