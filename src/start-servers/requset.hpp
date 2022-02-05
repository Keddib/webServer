#ifndef	__REQUEST__
#define __REQUEST__


#include "../configuration/confHeaders.hpp"
#include "../HandleRequest/ErrorGen.hpp"
#include "../MACROS.hpp"
#include <cstring>
#include <map>

class	Request
{
	private:
		int				connFD;
		bool				chunkedBodyState;
		std::fstream			bodyFileObj;
		long				hasBeenRead;
		std::string			bodyString;
		int				isChuncked; // -1 default 0 means that the header Transfer-Encoding has past but not value not chuncked 1 means chuncked
		long				bodySize; // keep eye on this type mybe body size will exceeds
		std::string			tmpStr;
		int				status;
		int				comServerIndex; // this wil  help to identify which servers should handel this request you will give this comServerIndex to map and you will get index to right commonServers class
		int				method; // allowed are GET DELETE POST
		std::string			aResourcPath;
		bool				version; // false if HTTP/1.0 true if HTTP/1.1
		std::string			aHostName;
		std::vector<std::pair<std::string, std::string> >	aHeaders;
		const char			*first, *second;
		char				*endStr;
		char				methodHolder[LONGEST_METHOD + 1];
		char				fileName[30];
		bool				booltmp;
		Response			*Restmp;
		ErrorGen			errorRespo;
	private:
		int				ProcessOneLine(char *str, long size);
		int				ProcessHeaders(char **str, int size);
		bool				IsHeadersDone(char **str);
		bool				FirstSecondFromHeaderLine(bool &opfold);
		void				TakeInfoFromHeaders(char **str);
		Response			*InitFromStartLine();
		bool				ProcessBody(char *str, long size);
		int				HowToReadBody() const;
		bool				ReadBody(char *str, long size);
		bool				ReadByChunck(char *str, long size);
		bool				BodyStringCase(char *str, long size);
		bool				BodyFileCase(char *str, long size);
		Response			*StartLineParsing(char **str, int &size);
		void				INIT_AT_CONSTRUCTION(int, int);
	public:
		Response 			*AddToRequest(char *str, int size);
		Request(const Request &cp);
		Request&			operator=(const Request &rhs);
		Request(int cf,int sFd);
};

#endif
