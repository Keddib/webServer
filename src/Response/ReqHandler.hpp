#ifndef REQ_HANDLER_HPP
#define REQ_HANDLER_HPP

#include "../Includes.hpp"
#include "../configuration/ServersInterface.hpp"
#include "../start-servers/requset.hpp"
#include "../Response/ResGenerator.hpp"
#include "Response.hpp"

extern ServersInterface ServI;
extern ResGenerator ResGen;


class ReqHandler {
	private:
		const Request &_req;
		Location &_location;
		std::string _reqResource;
		std::string _hostName;
		std::string _root;
		std::string _resource; // resource without QueryString
		std::string _hostPath;
		std::string _queryString;
		std::string _CGUextention;
		std::string _CGIpath;
		std::string _UPLOADpath;
		std::string _childOut;
		int _reqMethod;
		size_t _reqCMservers;
		time_t _cgiTimeOut;
		bool _reqHttpVersion;
		bool _connection;
		bool _autoIndex;
		bool _isCGI;
		bool _isUPLOAD;

	public:
		ReqHandler(const Request &req);
		~ReqHandler();
		Response *getResponse();
	private:
		ReqHandler& operator=(const ReqHandler &){ return *this; }
		int checkConnectionHeader();
		Response *MethodNotAllowed();
		Response *Redirect();
		Response *HandleDirResource();
		Response *HandleFileResource();
		std::string getIfModifiedSinceTime();
		Response *uploadFile();
		Response *HundleCGI();
		void setENV(std::vector<std::string> &);
		char **getENV(std::vector<std::string> &);
		void addRequestHeaders(std::vector<std::string> &envHeaders);
		void trasferReqHeaderToCGIheader(
			std::vector<std::string> &envHeaders,
			const std::pair<std::string, std::string> &Hfield );
		int excuteChildProcess(char **ENV, int inFD, int outFD, int &pid);
		void INIT();
};

#endif
