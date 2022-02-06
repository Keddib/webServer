#include "util.h"
#include "Response.hpp"
#include "../configuration/confHeaders.hpp"
#include "../start-servers/requset.hpp"
#include "ErrorGen.hpp"

extern ServersInterface ServI;
extern ErrorGen	errorRespo;

struct ReqInfo
{
	std::string tmpStr;
	std::vector<std::string> tmpHeader;
	const std::string 	&rsource_path;
	const std::string 	&host_name;
	int					com_srv_index;
	int					meth;
	bool				vers;

	ReqInfo(const std::string &rcp, const std::string &hn, int srvIndex, int mt, bool v) :
	rsource_path(rcp), host_name(hn)
	{
		com_srv_index = srvIndex;
		meth = mt;
		vers = v;
	}
};


Response *MethodsErrors(ReqInfo &Rq, const Location &rLoc)
{
	Rq.tmpHeader.reserve(1);
	Rq.tmpHeader.push_back("Allow: ");
	rLoc.fillAllowedMethods(Rq.tmpHeader[0]);
	Rq.tmpHeader[0] += "\r\n";
	return errorRespo.getResponse(Rq.com_srv_index, 405, Rq.host_name, Rq.tmpHeader);
}

Response* HandleRequest(const Request &req)
{
	ReqInfo Rq(
		req.getResource(),
		req.getHost(),
		req.getCommonServerIndex(),
		req.getMethod(),
		req.getVersion()
		);
	const Location &rLoc = ServI[Rq.com_srv_index].whichServer(Rq.host_name).whichLocation(Rq.rsource_path);
	if (!rLoc.isMethodAllowed(Rq.meth))
		return MethodsErrors(Rq, rLoc);
	// check if req.method is accepted on location
	// check http version if 0 set connection to close/ KA to 0;
	// if redirect return redirect response
	// if resource is file check if exist
		// if exist and not CGI script open file and return response
	return NULL;
}
