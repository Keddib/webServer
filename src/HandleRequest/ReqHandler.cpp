#include "utils.hpp"
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
	bool				keepAlive;

	ReqInfo(const std::string &rcp, const std::string &hn, int srvIndex, int mt, bool v) :
	rsource_path(rcp), host_name(hn)
	{
		com_srv_index = srvIndex;
		meth = mt;
		vers = v;
		keepAlive = true;
	}
};

Response *getUnusedCodeResponse(int code, const std::string &body)
{
	Response *res = new Response;
	size_t bsize = std::strlen(body.c_str());
	res->setStartLine("HTTP/1.1", code, getErrorMessage(code));
	res->setHeader("Content-Type: ", "application/octet-stream", 0);
	res->setHeader("Content-Length: ", std::to_string(bsize), 0);
	res->setBodySize(bsize);
	res->setHeader("Connection: ", "close", 1);
	res->setKeepAlive(0);
	res->addBodyToBuffer(body);
	return res;
}

Response *Redirect(ReqInfo &Rq, int code, std::string URI)
{
	//edit HostName
	std::string Host = Rq.host_name;
	if (Host.empty()) // convert to IP to X.X.X.X format
			Host = getMyIP();
	// edit URI
	if (URI[0] == '/')
		URI = std::string("http://") + Host + URI;
	// if code not valid Redirect
	if (code > 303 && code < 307)
		return getUnusedCodeResponse(code, URI);

	Rq.tmpHeader.reserve(1);
	Rq.tmpHeader.push_back("Location: ");
	Rq.tmpHeader[0] += URI + "\r\n";
	return errorRespo.getResponse(Rq.com_srv_index, code, Rq.host_name, Rq.keepAlive, Rq.tmpHeader);
}

Response *MethodsErrors(ReqInfo &Rq, const Location &rLoc)
{
	Rq.tmpHeader.reserve(1);
	Rq.tmpHeader.push_back("Allow: ");
	rLoc.fillAllowedMethods(Rq.tmpHeader[0]);
	Rq.tmpHeader[0] += "\r\n";
	return errorRespo.getResponse(Rq.com_srv_index, 405, Rq.host_name, Rq.keepAlive, Rq.tmpHeader);
}

Response *FileFound200(const std::string &PATH, FileInfo &Fdata, int server)
{
	Response *res = new Response();
	res->setCommonServerIndex(server);
	res->setStartLine("HTTP/1.1", 200, "OK");
	if (!Fdata.keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	res->setHeader("Content-Type", Fdata.Ftype);
	res->setHeader("Content-Length", std::to_string(Fdata.size));
	res->setBodySize(Fdata.size);
	res->setHeader("Accept-Ranges", "none");
	res->setHeader("Last-Modified", Fdata.Mtime, 1);
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
	Fdata.keepAlive = Rq.keepAlive;
	//std::cout << PATH << '\n';
	int ret = getFileInfo(PATH, Fdata);
	if (ret == 0) // found
	{
		// if Rq.method == GET && 	If-Modified-Since header is present
		// and it's value equal to Fdata.Lmodified return 304 (redirect to cache)
		return FileFound200(PATH, Fdata, Rq.com_srv_index);
	}
	else if (ret == 1) // not found
		return (errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name, Rq.keepAlive));
	else
		return (errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name, Rq.keepAlive));
		// forbiden
}

Response *GetDirListingResponse(const std::string &PATH, ReqInfo &Rq)
{
	Response *res = new Response();
	res->setStartLine("HTTP/1.1", 200, "OK");
	res->setCommonServerIndex(Rq.com_srv_index);
	if (!Rq.keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	res->setHeader("Content-Type", "text/html");
	const std::string &s = ListDirectory(PATH, Rq.rsource_path);
	size_t size = s.size();
	res->setBodySize(size);
	res->setHeader("Content-Length", std::to_string(size), 1);
	res->addBodyToBuffer(s);
	// content lenght
	return res;
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
	if (error == 1 || (index.empty() && !Rq.indexon)) // dir not found
		return errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name, Rq.keepAlive);
	else if (error == 2) // dir forbiden
		return errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name, Rq.keepAlive);
	else if (index.empty() && Rq.indexon) // dir listing
		return GetDirListingResponse(PATH, Rq); // need to return a response with directory listing
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


int checkConnectionHeader(const std::vector<std::pair<std::string, std::string> > &headers)
{
	size_t size = headers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!strcmp(headers[i].first.c_str(), "Connection"))
		{
			if (!strcmp(headers[i].second.c_str(), "close"))
				return 0;
			else
				return 1;
		}
	}
	return 2;
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

	// print start line
	const Location &rLoc = ServI[Rq.com_srv_index].whichServer(Rq.host_name).whichLocation(Rq.rsource_path);
	// set keepAlive

	// check if req.method is accepted on location
	if (!rLoc.isMethodAllowed(Rq.meth))
		return MethodsErrors(Rq, rLoc);
	Rq.indexon = rLoc.isAutoIndexOn();

	// check connection
	int ret = checkConnectionHeader(req.aHeaders);
	if (ret == 2)
		Rq.keepAlive = Rq.vers ? 1 : 0;
	else
		Rq.keepAlive = ret;
	// if redirect return redirect response
	// return -> ??
	if (rLoc.isRedirect())
		return Redirect(Rq, rLoc.getRedirectCode(), rLoc.getRedirectURI());
	// if exist and not CGI script open file and return response
	// if (CGI) -> ??

	// find position of query string so we don't concatinate it with root
	size_t pos = Rq.rsource_path.find_last_of('?');

	// concatinate root with resource
	std::string root = rLoc.getRoute();
	if (root.empty())
		root = "../www";
	std::string PATH = root + Rq.rsource_path.substr(0, pos);

	std::cout << PATH << '\n';
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
