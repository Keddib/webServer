#include "utils.hpp"
#include "Response.hpp"
#include "../configuration/confHeaders.hpp"
#include "../start-servers/requset.hpp"
#include "ErrorGen.hpp"
#include "CGI/CGI.hpp"

extern ServersInterface ServI;

extern ErrorGen	errorRespo;

Response *getUnusedCodeResponse(int code, const std::string &body)
{
	Response *res = new Response;
	size_t bsize = std::strlen(body.c_str());
	res->setStartLine("HTTP/1.1", code, getErrorMessage(code));
	res->setHeader("Content-Type: ", "application/octet-stream", 0);
	res->setHeader("Content-Length: ", to_string(bsize), 0);
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
	res->setHeader("Content-Length", to_string(Fdata.size));
	res->setBodySize(Fdata.size);
	res->setHeader("Accept-Ranges", "none");
	res->setHeader("Last-Modified", Fdata.Mtime, 1);
	res->setBodyfile(PATH);
	// res->display();
	return res;
}

bool isFileNotModified(ReqInfo &Rq, FileInfo& Fdata)
{
	if (Rq.meth == GET)
	{
		std::string tmpTime(Fdata.Mtime);
		std::transform(tmpTime.begin(), tmpTime.end(), tmpTime.begin(), asciitolower);
		if (!std::strcmp(Rq.cachHeader.c_str(), tmpTime.c_str()))
			return true;
	}
	return false;
}

Response *deleteFile(const std::string &PATH, ReqInfo &Rq)
{
	remove(PATH.c_str());
	Response *res = new Response();
	res->setCommonServerIndex(Rq.com_srv_index);
	res->setStartLine("HTTP/1.1", 204, "No Content");
	if (!Rq.keepAlive)
	{
		res->setHeader("Connection", "close", 1);
		res->setKeepAlive(false);
	} else
		res->setHeader("Connection", "keep-alive", 1);
	// res->display();
	return res;
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
	res->setHeader("Content-Length", to_string(size), 1);
	res->addBodyToBuffer(s);
	// content lenght
	return res;
}

Response *HundleCGI(const Request &req, const ReqInfo &Rq)
{
	// file already execte
	try {
		CGII CGIhundler(req, Rq);
		CGIhundler.setENV();
		// CGIhundler.display();
		return CGIhundler.getResponse();
	} catch ( ... ) {
			return errorRespo.getResponse(Rq.com_srv_index, 500, Rq.host_name, Rq.keepAlive);
	}

}

Response *HandleFileResource(const Request &req, ReqInfo &Rq)
{
	FileInfo Fdata;
	Fdata.keepAlive = Rq.keepAlive; // added for 304 response
	int ret = getFileInfo(Rq.PATH, Fdata);
	if (ret == 0) // found
	{
		if (Rq.isCGI && fileHasextension(Rq.PATH, Rq.cgiPATH)) // hundle CGI
		{
			Rq.CGIfile = Rq.PATH.substr(Rq.PATH.find_last_of('/') + 1);
			return HundleCGI(req, Rq);
		}
		if (Rq.meth == DELETE)
			return deleteFile(Rq.PATH, Rq);
		// if file is cachable and not medified return 304 (redirect to cache)
		if (isFileNotModified(Rq, Fdata)) // return true if file is not modified
			return errorRespo.get304Respone(Rq.com_srv_index, Fdata.Mtime, Rq.keepAlive);
		return FileFound200(Rq.PATH, Fdata, Rq.com_srv_index);
	}
	else if (ret == 1) // not found
		return (errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name, Rq.keepAlive));
	return (errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name, Rq.keepAlive));
}


Response *HandleDirResource(const Request &req, ReqInfo &Rq, const std::vector<std::string> &iVec)
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
	std::string index = lookForIndexInDirectory(Rq.PATH, iVec, error);
	if (index.empty() && !Rq.indexon) // index not found and no directory listing
		return errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name, Rq.keepAlive);
	else if (error == 2) // dir forbiden
		return errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name, Rq.keepAlive);
	else if ((index.empty() && Rq.indexon) || error == 1) // dir listing
		return GetDirListingResponse(Rq.PATH, Rq); // need to return a response with directory listing
	Rq.PATH += index;
	return HandleFileResource(req, Rq); // found
}

void getLocationIndexes(const Location &loc, std::vector<std::string> &indexes)
{
	indexes = loc.getIndexes();
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

std::string getCacheHeader(const std::vector<std::pair<std::string, std::string> > &headers)
{
	size_t size = headers.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!strcmp(headers[i].first.c_str(), "If-Modified-Since"))
			return headers[i].second;
	}
	return "";
}

Response* HandleRequest(const Request &req)
{
	ReqInfo Rq(
		req.getResource(),
		req.getHost(),
		req.getCommonServerIndex(),
		req.getMethod(),
		req.getVersion(),
		getCacheHeader(req.aHeaders)
		);

	// print start line
	const Location &rLoc = ServI[Rq.com_srv_index].whichServer(Rq.host_name).whichLocation(Rq.rsource_path);
	// rLoc.Display();
	// exit(1);
	// check connection
	// check http version if 0 && Keep_alive not excite set connection to close/ KA to 0;
	int ret = checkConnectionHeader(req.aHeaders);
	if (ret == 2)
		Rq.keepAlive = Rq.vers ? 1 : 0;
	else
		Rq.keepAlive = ret;

	// check if req.method is accepted on location
	if (!rLoc.isMethodAllowed(Rq.meth))
		return MethodsErrors(Rq, rLoc);
	Rq.indexon = rLoc.isAutoIndexOn();
	Rq.cgiPATH = rLoc.getCGIext();

	// if redirect return redirect response
	// return -> ??
	if (rLoc.isRedirect())
		return Redirect(Rq, rLoc.getRedirectCode(), rLoc.getRedirectURI());

	Rq.isCGI = rLoc.isCGI();

	// find position of query string so we don't concatinate it with root
	// to do file look up
	// concatinate root with resource
	size_t pos = Rq.rsource_path.find_last_of('?');
	Rq.PATH = rLoc.getRoute() + Rq.rsource_path.substr(0, pos);

	std::cout << Rq.PATH << '\n';

	// check if file or dir
	if (Rq.PATH[Rq.PATH.size() - 1] != '/') // is file
		return HandleFileResource(req, Rq);
	else // is directory
	{
		if (Rq.meth == DELETE && !Rq.isCGI) // if method is delete return not found
			return errorRespo.getResponse(Rq.com_srv_index, 404, Rq.host_name, Rq.keepAlive);
		std::vector<std::string> indexes;
		getLocationIndexes(rLoc, indexes);
		if (indexes.empty() && !Rq.indexon)
			return errorRespo.getResponse(Rq.com_srv_index, 403, Rq.host_name, Rq.keepAlive);
		return HandleDirResource(req, Rq, indexes);
	}
	return NULL;
}
