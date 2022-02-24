#include "ReqHandler.hpp"

ReqHandler::ReqHandler(const Request &req)
: _req(req),
_location(const_cast<Location &>(ServI[req.getCommonServerIndex()].
whichServer(req.getHost()).whichLocation(req.getResource())))
{
	INIT();
}

ReqHandler::~ReqHandler() {}

void ReqHandler::INIT()
{
	_reqResource = _req.getResource();
	_reqMethod = _req.getMethod();
	_reqCMservers = _req.getCommonServerIndex();
	_reqHttpVersion = _req.getVersion();
	_hostName = _req.getHost();
	_portStr = _req.getPortStr();
	_location = const_cast<Location &>(ServI[_req.getCommonServerIndex()].
	whichServer(_hostName).whichLocation(_reqResource ));
	_autoIndex = _location.isAutoIndexOn();
	_root = _location.getRoute();
	if ((_isCGI = _location.isCGI()))
	{
		_CGIpath = _location.getCGIpath();
		_CGUextention = _location.getCGIext();
		_cgiTimeOut = _location.getCGItimeOut();
	}
	if ((_isUPLOAD = _location.isUPLOAD()))
		_UPLOADpath = _location.getUPLOADpath();
	size_t pos = _reqResource.find_first_of('?');
	_resource = _reqResource.substr(0, pos);
	if (pos != std::string::npos)
		_queryString = _reqResource.substr(pos + 1);
	_hostPath = _root + _resource;
	_connection = true;
	int ret = checkConnectionHeader();
	if (ret == 2)
		_connection = _reqHttpVersion ? 1 : 0;
	else
		_connection = ret;
}

Response *ReqHandler::getResponse()
{
	std::cout <<"REQ: "<< _reqMethod << ' ' << _reqResource << '\n';
	std::cout <<"REQuri" <<_reqResource << "\n";
	std::cout <<"Res" <<_resource << "\n";
	std::cout <<"PATH" <<_hostPath << "\n";
	std::cout <<"QS"<<_queryString << "\n";

	// check if req.method is accepted on location
	if (!_location.isMethodAllowed(_reqMethod))
		return MethodNotAllowed();
	if (_location.isRedirect()) // is redirect
		return Redirect();
	if (!_isCGI && !_location.isUPLOAD()) // method not allowd for static serving
		if (_reqMethod == POST)
			return ResGen.getErrorResponse(_reqCMservers, 405, _hostName, _connection);
	// if (_isUPLOAD) // if upload Post need to create new resource
	// 	if (_reqMethod == POST)
	// 		return uploadFile();
	if (_hostPath[_hostPath.size() - 1] != '/') // is file
		return HandleFileResource();
	else // directory
	{
		if (_reqMethod == DELETE && !_isCGI) // if method is delete return not found
			return ResGen.getErrorResponse(_reqCMservers, 404, _hostName, _connection);
		if (_location.getIndexes().empty() && !_autoIndex)
			return ResGen.getErrorResponse(_reqCMservers, 403, _hostName, _connection);
		return HandleDirResource();
	}
	return NULL;
}

Response *ReqHandler::HandleFileResource()
{
	FileInfo Fdata;
	int ret = getFileInfo(_hostPath, Fdata);
	if (ret == 0) // found
	{
		if (_isCGI && fileHasextension(_hostPath, _location.getCGIext())) // hundle CGI
			return HundleCGI();
		if (_reqMethod == DELETE)
		{
			remove(_hostPath.c_str());
			return ResGen.get204Response(_reqCMservers, _connection);
		}
		return ResGen.get200Response(_reqCMservers, _hostPath, Fdata);
	}
	else if (ret == 1) // not found
		return (ResGen.getErrorResponse(_reqCMservers, 404, _hostName, _connection));
	return (ResGen.getErrorResponse(_reqCMservers, 403, _hostName, _connection));
}


Response *ReqHandler::HandleDirResource()
{
	int error(-1);
	std::string index = lookForIndexInDirectory(_hostPath, _location.getIndexes(), error);
	if ((index.empty() && !_autoIndex) || (error == 2)) // index not found and no directory listing
		return ResGen.getErrorResponse(_reqCMservers, 403, _hostName, _connection);
	else if (index.empty() && _autoIndex) // dir listing
		return ResGen.GetDirListingResponse(_hostPath, _resource, _reqCMservers, _connection); // need to return a response with directory listing
	else if (error == 1)
		return ResGen.getErrorResponse(_reqCMservers, 404, _hostName, _connection);
	_resource += index;
	_hostPath = _root + _resource;
	return HandleFileResource(); // handle resource with the index as resource

}

Response *ReqHandler::Redirect()
{
	std::string URI = _location.getRedirectURI();
	std::string host = _hostName;
	int code = _location.getRedirectCode();
	if (host.empty()) // appent host and port to redirect url
			host = getMyIP();
	if (URI[0] == '/')
		URI = std::string("http://") + host + URI;
	std::string Header("Location: ");
	Header += URI + "\r\n";
	return ResGen.getRedirecResponse(_reqCMservers, code, Header, _hostName, _connection);
}

Response *ReqHandler::MethodNotAllowed()
{
	std::string header("Allow: ");
	_location.fillAllowedMethods(header);
	header += "\r\n";
	return ResGen.getErrorResponse(_reqCMservers, 405, _hostName, _connection, header);
}

int ReqHandler::checkConnectionHeader()
{
	size_t size = _req.aHeaders.size();
	for (size_t i = 0; i < size; i++)
	{
		if (!strcmp(_req.aHeaders[i].first.c_str(), "Connection"))
		{
			if (!strcmp(_req.aHeaders[i].second.c_str(), "close"))
				return 0;
			else
				return 1;
		}
	}
	return 2;
}

int ReqHandler::excuteChildProcess(char **ENV, int inFD, int outFD, int &pid)
{
	pid = fork();
	if (pid < 0)
		return (-1);
	if (pid == 0) // child
	{
		char *args[3];
		args[0] = (char *)_CGIpath.c_str();
		args[1] = (char *)_hostPath.c_str();
		args[2] = NULL;
		std::cerr << "inside child\n";
		// change directory to the the scripte directory
		chdir(_hostPath.substr(0, _hostPath.find_last_of('/')).c_str());
		// Child. Start by closing descriptors we don't need in this process
		if (dup2(outFD, 1) == -1 || dup2(inFD, 0) == -1)
			exit(500);
		if ((inFD != 0 && close(inFD)) || close(outFD))
			exit(500);
		execve(args[0], args, ENV);
		exit(502);
	}
	return 0;
}

#include <sys/time.h>
std::string createFileName()
{
	struct timeval t;
	gettimeofday(&t, NULL); // get current time
	return std::string("/tmp/.") + to_string(1000000 * t.tv_sec + t.tv_usec);
}

Response *ReqHandler::HundleCGI()
{
	int PID;
	_childOut = createFileName();
	std::vector<std::string> envHeaders;
	setENV(envHeaders);
	char **ENV = getENV(envHeaders);
	int inFD = 0;
	if (_reqMethod == POST) {
		inFD = open(_req.getFileName(), O_RDONLY);
		if (inFD == -1)
			return ResGen.getErrorResponse(_reqCMservers, 500, _hostName, _connection);
	}
	int outFD = open(_childOut.c_str(), O_WRONLY | O_CREAT, 0666);
	if (outFD == -1)
		return ResGen.getErrorResponse(_reqCMservers, 500, _hostName, _connection);
	if (excuteChildProcess(ENV, inFD, outFD, PID))
			return ResGen.getErrorResponse(_reqCMservers, 500, _hostName, _connection);
	if (inFD != 0 && close(inFD))
		return ResGen.getErrorResponse(_reqCMservers, 500, _hostName, _connection);
	if (close(outFD))
			return ResGen.getErrorResponse(_reqCMservers, 500, _hostName, _connection);
	freeArray(ENV);
	Response *res = new Response();
	res->setKeepAlive(_connection);
	res->setCommonServerIndex(_reqCMservers);
	res->setTimeout(_cgiTimeOut);
	res->setHost(_hostName);
	res->setNotReady(std::time(NULL), PID, _childOut);
	return res;
}

void ReqHandler::setENV(std::vector<std::string> &envHeaders)
{
	envHeaders.push_back("REQUEST_URI=" + _reqResource);
	envHeaders.push_back("DOCUMENT_URI=" + _resource);
	envHeaders.push_back("SCRIPT_NAME=" + _resource);
	envHeaders.push_back("SCRIPT_FILENAME=" + _hostPath);
	envHeaders.push_back("PATH_TRANSLATED=" + _hostPath);
	envHeaders.push_back("QUERY_STRING=" + _queryString);
	envHeaders.push_back("SERVER_NAME=" + _hostName);
	envHeaders.push_back("SERVER_PORT=" + _portStr);
	envHeaders.push_back("REQUEST_METHOD=" + get_method(_reqMethod));
	envHeaders.push_back("DOCUMENT_ROOT=" + _root);
	envHeaders.push_back("GETAWAY_INTERFACE=CGI/1.1");
	envHeaders.push_back("SERVER_PROTOCOL=HTTP/1.1");
	envHeaders.push_back("REDIRECT_STATUS=200");
	envHeaders.push_back("FCGI_ROLE=RESPONDER");
	envHeaders.push_back("REQUEST_SCHEME=http");
	envHeaders.push_back("SERVER_SOFTWARE=webserv/1.1 " + getOsName());
	envHeaders.push_back("CONTENT_LENGTH=" + to_string(_req.getBodySize()));
	envHeaders.push_back("PATH=" + std::string(std::getenv("PATH")));
	const std::pair<std::string, int> &remoteInfo = _req.GetClientInfo();
	envHeaders.push_back("REMOTE_ADDR=" + remoteInfo.first);
	envHeaders.push_back("REMOTE_PORT=" + to_string(remoteInfo.second));
	addRequestHeaders(envHeaders);
		// for (int i = 0; i < envHeaders.size(); i++)
		// 	std::cout << envHeaders[i] + '\n';
}

void ReqHandler::addRequestHeaders(std::vector<std::string> &envHeaders)
{
	size_t size = _req.aHeaders.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_req.aHeaders[i].first == "Content-Type")
			envHeaders.push_back("CONTENT_TYPE=" + _req.aHeaders[i].second);
		else if (_req.aHeaders[i].first == "Authorization" || _req.aHeaders[i].first == "Content-Length")
			continue;
		else
			trasferReqHeaderToCGIheader(envHeaders, _req.aHeaders[i]); // trasfer request header to CGI header and add it as an env variable
	}
}

void ReqHandler::trasferReqHeaderToCGIheader(
	std::vector<std::string> &envHeaders,
	const std::pair<std::string, std::string> &Hfield )
{
	// transfer Hfeild name to UPPERCASE_ and add prefix HTTP_
	std::string FieldName("HTTP_");
	FieldName += Hfield.first;
	std::transform(FieldName.begin(), FieldName.end(), FieldName.begin(), asciiToUpper);
	FieldName.push_back('=');
	envHeaders.push_back(FieldName + Hfield.second);
}

char **ReqHandler::getENV(std::vector<std::string> &envHeaders)
{
	char **ENV;
	size_t hSize = envHeaders.size() + 1;
	ENV = new char* [hSize];
	for (size_t i = 0; i < hSize - 1; i++)
	{
		size_t size = envHeaders[i].length();
		ENV[i] = new char[size + 1];
		std::memmove(ENV[i], envHeaders[i].c_str(), size + 1);
	}
	ENV[hSize - 1] = NULL;
	return ENV;
}
