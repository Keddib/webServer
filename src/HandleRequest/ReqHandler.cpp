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
	bool				indexon;

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

Response *FileFound200(const std::string &PATH, FileInfo &Fdata)
{
	Response *res = new Response();
	res->setStartLine("HTTP/1.1", 200, "OK");
	res->setHeader("Content-Type", Fdata.Ftype, 0);
	res->setHeader("Content-Length", std::to_string(Fdata.size), 0);
	res->setBodySize(Fdata.size);
	res->setHeader("Last-Modified", Fdata.Mtime, 0);
	res->setBodyfile(PATH);
	// res->display();
	return res;
}

Response *HandleFileResource(const std::string &PATH, ReqInfo &Rq)
{
	/*
	** if file and existe get file info and rerurn response with file in body
	** if can't access file return forbiden
	** if file not found return Not Found
	*/
	FileInfo Fdata;
	std::cout << PATH << '\n';
	int ret = getFileInfo(PATH, Fdata);
	if (ret == 0) // found
		return FileFound200(PATH, Fdata);
	else if (ret == 1) // not found
		return (errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name));
	else
		return (errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name));
		// forbiden
}

Response *HandleDirResource(std::string &PATH, ReqInfo &Rq, const std::vector<std::string> &iVec)
{
	/*
	** if Dir and existe look for indexes within that file
	** if no index found and autoindex is on list directory cotent
	** if no index found and autoindex is off return Not found
	** if index found go back to HandleFileResouce
	** if can't access Dir return forbiden
	** if Dir not found return Not Found
	*/
	int error(-1);
	std::string index = lookForIndexInDirectory(PATH, iVec, error);
	if (error == 1 || index.empty()) // not found
		return errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name);
	else if (error == 2) // forbiden
		return errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name);
	return HandleFileResource(PATH += index, Rq); // found
}

void getLocationIndexes(const Location &loc, std::vector<std::string> &indexes)
{
	const std::vector<std::string> &vec = loc.getIndexes();
	if (vec.empty())
	{
		indexes.push_back("index.html");
		indexes.push_back("index.htm");
		return;
	}
	indexes = vec;
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
	// check if req.method is accepted on location
	if (!rLoc.isMethodAllowed(Rq.meth))
		return MethodsErrors(Rq, rLoc);
	Rq.indexon = rLoc.isAutoIndexOn();
	// if redirect return redirect response
	// return -> ??
		// if exist and not CGI script open file and return response
	// if (CGI) -> ??

	// find position of query string so we don't concatinate it with root
	size_t pos = Rq.rsource_path.find_last_of('?');

	// concatinate root with resource
	std::string PATH = rLoc.getRoute() + Rq.rsource_path.substr(0, pos);

	// check if file or dir
	if (PATH[PATH.size()-1] != '/') // is file
		return HandleFileResource(PATH, Rq);
	else // is directory
	{
		// get indexes
		std::vector<std::string> indexes;
		getLocationIndexes(rLoc, indexes);
		return HandleDirResource(PATH, Rq, indexes);
	}

	// check http version if 0 && Keep_alive not excite set connection to close/ KA to 0;
	return NULL;
}
