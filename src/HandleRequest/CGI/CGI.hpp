#ifndef CGI_77_HPP
#define CGI_77_HPP

#include "../utils.hpp"
#include "../Response.hpp"
// #include "../ErrorGen.hpp"
#include "../../start-servers/requset.hpp"
#include <signal.h>
#include <sys/wait.h>
#include <cctype>

int	IndexOf(const char *str, char c);
bool	my_strncmp(const char *s1, const char *s2, size_t n);

extern ServersInterface ServI;

extern ErrorGen	errorRespo;

extern bool DONE;

struct CGIIresInfo{
	CGIIresInfo() : cont_type(false), location(false) { status.first = false; status.second = 200; bodyExist = false;}

	bool							cont_type;
	bool							location;
	std::pair<short, short>			loc_info;
	std::pair<bool, short>			status;
	short							bodyExist;
};


class CGII
{
	private:
		unsigned int 	bodySize;
		/* data */
		std::fstream _CGIres;
		const Request &_req;
		const Location &_Loc;
		char **_ENV;
		std::vector<std::string> _Headers;
		const ReqInfo &_Rq;
		char *_args[3];
		std::string _PATH;
		time_t _sTime;
		time_t CGItimeOut;
		pid_t c_pid;
		char _buff[BUFFER_SIZE];
		CGIIresInfo	_cgii_res_info;
		std::string _CGIfile;
		std::vector<std::string> _CGIheaders;
	private:
		CGII& operator=(const CGII &){return *this;}
		char **getENV();
		void trasferReqHeaderToCGIheader(const std::pair<std::string, std::string> &);
		void addRequestHeaders();
		std::string get_method(int method) const;
		std::string getQueryString(std::string &resource) const;
		bool isHeaderServerSpecific(const std::string &) const;
		Response *cgiError(int code);
		int excuteChildProcess(int Rfd[], int Wfd[]);
		int SendFile(int fd);
		int ReadCGIresponse(int fd);
		Response *ParseCGIresponse(const std::string &CGIfileRespone);
		Response	*ResponseConstruction();
		Response	*DocumentResponse();
		Response	*ClientRedirectResponse();
	public:
		int ErrorCode;
		CGII(const Request &req, const ReqInfo &Rq);
		~CGII();
		void setENV();
		Response *getResponse();
		// void display() const;
};

#endif
