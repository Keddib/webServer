#ifndef CGI_77_HPP
#define CGI_77_HPP

#include "../utils.hpp"
#include "../Response.hpp"
// #include "../ErrorGen.hpp"
#include "../../start-servers/requset.hpp"

extern ServersInterface ServI;

extern ErrorGen	errorRespo;

class CGII
{
	private:
		/* data */
		const Request &_req;
		const Location &_Loc;
		char **_ENV;
		std::vector<std::string> _Headers;
		const ReqInfo &_Rq;
		std::string _PATH;
		std::string _CGI;
	private:
		CGII& operator=(const CGII &){return *this;}
		char **getENV();
		void trasferReqHeaderToCGIheader(const std::pair<std::string, std::string> &);
		void addRequestHeaders();
		std::string get_method(int method) const;
		std::string getQueryString(std::string &resource) const;
		bool isHeaderServerSpecific(const std::string &) const;
		Response *cgiError(int code);
		int CGII::excuteChildProcess(int Rfd[], int Wfd[]);
	public:
		int ErrorCode;
		CGII(const Request &req, const ReqInfo &Rq);
		~CGII();
		void setENV();
		Response *getResponse();
		void display() const;
};

#endif
