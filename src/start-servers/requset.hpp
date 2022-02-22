#ifndef	__REQUEST__
#define __REQUEST__


#include "../Includes.hpp"
#include "../Response/ResGenerator.hpp"
#include "../MACROS.hpp"
#include <cstring>
#include <ctime>
#include <cctype>
#include <map>


extern ResGenerator ResGen;

class	Request
{
	private:
		unsigned int			max_client_size;
		unsigned int			totalRead;
		std::pair<std::string, in_port_t>	client_info;
		bool					is_req_alive;
		std::time_t				startTime;
		int						connFD;
		bool					chunkedBodyState;
		std::fstream			bodyFileObj;
		long					hasBeenRead;
		std::string				bodyString;
		int						isChuncked; // -1 default 0 means that the header Transfer-Encoding has past but not value not chuncked 1 means chuncked
		long					bodySize; // keep eye on this type mybe body size will exceeds
		std::string				tmpStr;
		int						status;
		int						comServerIndex; // this wil  help to identify which servers should handel this request you will give this comServerIndex to map and you will get index to right commonServers class
		int						method; // allowed are GET DELETE POST
		std::string				aResourcPath;
		std::string				tmpRSP;
		bool					version; // false if HTTP/1.0 true if HTTP/1.1
		std::string				aHostName;
		const char				*first, *second;
		char					*endStr;
		char					methodHolder[LONGEST_METHOD + 1];
		char					fileName[30];
		bool					booltmp;
		Response				*Restmp;
	private:
		void					DISPLAY();
		void					ResourseDecoding();
		int						ProcessOneLine(char *str, long size);
		Response				*ProcessHeaders(char **str, int size);
		Response				*ReserveSpaceForBody();
		bool					IsHeadersDone(char **str);
		Response				*FirstSecondFromHeaderLine(bool &opfold);
		Response				*TakeInfoFromHeaders(char **str);
		Response				*InitFromStartLine();
		bool					ProcessBody(char *str, long size);
		int						HowToReadBody() const;
		bool					ReadBody(char *str, long size);
		bool					ReadByChunck(char *str, long size);
		bool					BodyStringCase(char *str, long size);
		bool					BodyFileCase(char *str, long size);
		Response				*StartLineParsing(char **str, int &size);
		void					INIT_AT_CONSTRUCTION(int, int);
		void					InsertCharToFile(char*, long);
	public:
		Request(const Request &cp);
		Request(int cf,int sFd, const struct sockaddr_in &);
		Request&				operator=(const Request &rhs);
		void					RESET();
		void					DisableRequest();
		bool					isStillValid(std::time_t);
		Response 				*AddToRequest(char *str, int size);
		int 					getCommonServerIndex() const;
		int 					getMethod() const ;
		const 					std::string& getResource() const;
		const char				*getFileName() const;
		bool 					getVersion() const;
		const std::string& 			getHost() const;
		const std::string& 			getBody() const;
		const std::fstream&			getBodyFile() const;
		bool					isBodyStr() const;
		long					getBodySize() const;
		const std::pair<std::string, in_port_t>	GetClientInfo() const;
		std::vector<std::pair<std::string, std::string> >	aHeaders;
};


Response* HandleRequest(const Request &req);

#endif
