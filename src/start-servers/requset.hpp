#ifndef	__REQUEST__
#define __REQUEST__


#include "../configuration/confHeaders.hpp"
#include "methods.hpp"

class	Request
{
	private:
		int				isChuncked; // -1 default 0 means that the header Transfer-Encoding has past but not value not chuncked 1 means chuncked
		long			bodySize; // keep eye on this type mybe body size will exceeds
		std::string			tmpStr;
		int				status;
		int				srvFd; // this wil  help to identify which servers should handel this request you will give this srvFd to map and you will get index to right commonServers class
		int				method; // allowed are GET DELETE POST
		std::string			aResourcPath;
		bool				version; // false if HTTP/1.0 true if HTTP/1.1
		std::string			aHostName;
		std::vector<std::pair<std::string, std::string> >	aHeaders;
	public:
		const char			*first, *second;
		char				methodHolder[LONGEST_METHOD + 1];
		int				ProcessOneLine(char *str, int size);
		int				ProcessHeaders(char *str, int size);
		bool				InitFromStartLine();
		int				AddToRequest(char *str, int size);
		Request(int sFd);
};

#endif
