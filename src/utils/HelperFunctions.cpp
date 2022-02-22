#include "../Includes.hpp"
#include "../MACROS.hpp"
#include "../configuration/ServersInterface.hpp"

extern ServersInterface ServI;

void getErrorPage(int error, std::string &page)
{
	std::string TOKEN = to_string(error) + " " + getErrorMessage(error);
	page += std::string(HTML1) + TOKEN + HTML2 + TOKEN + HTML3;
}

const char* getErrorMessage(int error)
{
	if (error == 200) return "OK";
	else if (error == 204) return "No Content";
	else if (error == 301) return "Moved Permanently";
	else if (error == 302) return "Found";
	else if (error == 303) return "See Other";
	else if (error == 304) return "Not Modified";
	else if (error == 307) return "Temporary Redirect";
	else if (error == 308) return "Permanent Redirect";
	else if (error == 400) return "Bad Request";
	else if (error == 403) return "Forbidden";
	else if (error == 404) return "Not Found";
	else if (error == 405) return "Method Not Allowed";
	else if (error == 406) return "Not Acceptable";
	else if (error == 408) return "Request Timeout";
	else if (error == 411) return "Length Required";
	else if (error == 413) return "Payload Too Large";
	else if (error == 500) return "Internal Server Error";
	else if (error == 501) return "Not Implemented";
	else if (error == 502) return "Bad Gateway";
	else if (error == 504) return "Gateway Timeout";
	else if (error == 505) return "HTTP Version Not Supported";
	return "";
}

std::string getOsName()
{
	#ifdef __APPLE__
	return "(Mac OSX)";
	#elif __MACH__
	return "(Mac OSX)";
	#elif __linux__
	return "(Linux)";
	#elif __FreeBSD__
	return "(FreeBSD)";
	#elif __unix || __unix__
	return "(Unix)";
	#else
	return "Other";
	#endif
}

std::string getDate()
{
	char buf[100];
	time_t now = std::time(0);
	struct tm *tm = std::gmtime(&now);
	std::strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", tm);
	return std::string(buf);
}

std::string getMyIP()
{
	char szBuffer[1024];
	if(gethostname(szBuffer, sizeof(szBuffer)) == -1)
		return "";
	struct hostent *host = gethostbyname(szBuffer);
	if(host == NULL)
		return "";
	//Obtain the computer's IP
	struct in_addr addr;
	memcpy(&addr, host->h_addr_list[0], sizeof(struct in_addr));
	std::string IP(inet_ntoa(addr));
	return IP;
}


std::string get_method(int meth)
{
	if (meth == GET) return "GET";
	else if (meth == POST) return "POST";
	else if (meth == DELETE) return "DELETE";
	return "";
}

std::string getServerPort(size_t server)
{
	int Port = ServI[server].GetIpPortCommonServer().second;
	return to_string(Port);
}

void freeArray(char **arr)
{
	int i = 0;
	while (arr[i])
	{
		delete[] arr[i];
		++i;
	}
	delete [] arr;
}
