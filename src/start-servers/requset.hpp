#ifndef	__REQUEST__
#define __REQUEST__


#include "../configuration/confHeaders.hpp"
#include "methods.hpp"
#include <cstring>
#include <map>
class	Request
{
	private:
		bool			chunkedBodyState;
		std::fstream	bodyFileObj;
		long			hasBeenRead;
		int				bodyFd;
		std::string		bodyString;
		int				isChuncked; // -1 default 0 means that the header Transfer-Encoding has past but not value not chuncked 1 means chuncked
		long			bodySize; // keep eye on this type mybe body size will exceeds
		std::string		tmpStr;
		int				status;
		int				srvFd; // this wil  help to identify which servers should handel this request you will give this srvFd to map and you will get index to right commonServers class
		int				method; // allowed are GET DELETE POST
		std::string		aResourcPath;
		bool			version; // false if HTTP/1.0 true if HTTP/1.1
		std::string		aHostName;
		std::vector<std::pair<std::string, std::string> >	aHeaders;
		const char			*first, *second;
		char				*endStr;
		char				methodHolder[LONGEST_METHOD + 1];
		char				fileName[30];
	private:
		int					ProcessOneLine(char *str, long size);
		int					ProcessHeaders(char **str, int size);
		bool				IsHeadersDone(char **str);
		bool				FirstSecondFromHeaderLine(bool &opfold);
		void				TakeInfoFromHeaders(char **str);
		bool				InitFromStartLine();
		bool				ProcessBody(char *str, long size);
		int					HowToReadBody() const;
		bool				ReadBody(char *str, long size);
		bool				ReadByChunck(char *str, long size);
		bool				BodyStringCase(char *str, long size);
		bool				BodyFileCase(char *str, long size);
	public:
		int					AddToRequest(char *str, int size);
		Request(int sFd);
};

#endif
