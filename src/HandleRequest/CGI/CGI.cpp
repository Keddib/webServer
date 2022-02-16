#include "CGI.hpp"



CGII::CGII(const Request &req, const ReqInfo &Rq)
: _req(req),
_Loc(ServI[req.getCommonServerIndex()].whichServer(req.getHost()).whichLocation(req.getResource())) ,
_ENV(NULL),
_Rq(Rq)
{
	_Headers.reserve(13);

}

CGII::~CGII()
{
	size_t size = _Headers.size();
	for (int i = 0; i < size; i++)
		if (_ENV[i])
			delete _ENV[i];
	delete _ENV;
}

void CGII::setENV()
{
	std::cout << "inside setENV\n";
	std::string resource = _req.getResource();

	_Headers.push_back("REQUEST_URI=" + resource);
	std::string QS = getQueryString(resource); // this func remove the QS from resource
	_PATH = _Loc.getRoute() + resource;
	_Headers.push_back("SCRIPT_NAME=" + resource);
	_Headers.push_back("PATH_INFO=" + resource);
	_Headers.push_back("SCRIPT_FILENAME=" + _PATH);
	_Headers.push_back("PATH_TRANSLATED=" + _PATH);
	_Headers.push_back("QUERY_STRING=" + QS);
	_Headers.push_back("SERVER_NAME=" + _req.getHost());
	_Headers.push_back("REQUEST_METHOD=" + get_method(_req.getMethod()));
	_Headers.push_back("GETAWAY_INTERFACE=CGI/1.1");
	_Headers.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_Headers.push_back("SERVER_SOFTWARE=webserv/1.1 " + getOsName());
	int Port = ServI[_req.getCommonServerIndex()].GetIpPortCommonServer().second;
	_Headers.push_back("SERVER_PORT=" + to_string(Port));
	// remote client info
	const std::pair<std::string, int> &remoteInfo = _req.GetClientInfo();
	_Headers.push_back("REMOTE_ADDR=" + remoteInfo.first);
	// _Headers.push_back("REMOTE_PORT=" + to_string(remoteInfo.second));
	if(const char* env_p = std::getenv("PATH"))
		_Headers.push_back("PATH=" + std::string(env_p));
	// need only content-type and content-lenght
	if (_req.getMethod() == POST)
		_Headers.push_back("CONTENT_LENGTH=" + to_string(_req.getBodySize()));
	addRequestHeaders();
	getENV();
}

int CGII::excuteChildProcess(int Rfd[], int Wfd[])
{
	pid_t pid= pid = fork();
	if (pid < 0)
		return (0);
	// if the pid is zero, this is the child process
	if (pid == 0) // child
	{
		// Child. Start by closing descriptors we
		//  don't need in this process
		if (close(Rfd[READ]) || close(Wfd[WRITE]))
			exit(500);
		if (dup2(Rfd[WRITE], 1) == -1 || dup2(Wfd[READ], 0))
			exit(500);
		if (close(Wfd[READ]) || close(Rfd[WRITE]))
			exit(500);
		// execve(argv[0], argv, NULL);
		exit(502);
	}
	// // Parent. close unneeded descriptors
	close(Rfd[WRITE]);
	close(Wfd[READ]);
	return 0;
}

Response *CGII::getResponse()
{
/*	int Rfd[2]; // pipe for parent to read child output
	int Wfd[2]; // pipe for child to read parent output
	if (pipe(Rfd) < 0 || pipe(Wfd) < 0)
		return cgiError(500);
	if (!excuteChildProcess(Rfd, Wfd))
		return cgiError(500);
	// if request is post write body to Wfd[write], otherwise close the writing end of pipe
	// send a value to the child
	int wRet = write(Wfd[WRITE], "HELLO CHILD\n", 12);
	if (wRet < 0)
		return cgiError(500);
	close(Wfd[WRITE]);
	// now wait for a response
	// before reading the response wait for child to finish
	// wait for child termination
	wait(NULL); // if child didn't finish in 60s , kill child and return time out response
	// then read out put and parse it
	char buffer[100];
	int len = read(Rfd[READ], buffer, sizeof(buffer));
	if (len < 0)
		return cgiError(500);
	// else if (len == 0) if no output is supplied check exit status code of child
	close(Rfd[READ]); // close read end of pipe
	// parse the CGI response
	return NULL; // return ParseCGIresponse(buffer);*/
	return NULL;
}

void CGII::addRequestHeaders()
{
	size_t size = _req.aHeaders.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_req.aHeaders[i].first == "Content-Type")
			_Headers.push_back("CONTENT_TYPE=" + _req.aHeaders[i].second);
		else if (isHeaderServerSpecific(_req.aHeaders[i].first))
			continue;
		else
			trasferReqHeaderToCGIheader(_req.aHeaders[i]); // trasfer request header to CGI header and add it as an env variable
	}
}


void CGII::trasferReqHeaderToCGIheader(const std::pair<std::string, std::string> &Hfield)
{
	// transfer Hfeild name to UPPERCASE_ and add prefix HTTP_
	std::string FieldName("HTTP_");
	FieldName += Hfield.first;
	std::transform(FieldName.begin(), FieldName.end(), FieldName.begin(), asciiToUpper);
	FieldName.push_back('=');
	_Headers.push_back(FieldName + Hfield.second);
}

bool CGII::isHeaderServerSpecific(const std::string &hName) const
{
	if (hName == "Authorization" || hName == "Content-Length" || hName == "Connection")
		return true;
	return false;
}

	// get query string
std::string CGII::getQueryString(std::string &resource) const
{
	std::string QS;
	size_t pos = resource.find_last_of('?');
	if (pos != std::string::npos)
	{
		QS = resource.substr(pos + 1);
		resource.erase(pos);
	}
	return QS;
}

std::string CGII::get_method(int meth) const
{
	if (meth == GET) return "GET";
	else if (meth == POST) return "POST";
	else if (meth == DELETE) return "DELETE";
	return "";
}


void CGII::display() const
{
	std::cout << "inside display\n";
	size_t s = _Headers.size();
	std::cout << "size of header seted in CGI " << s << '\n';
	for (size_t i = 0; i < s; i++)
		std::cout << _Headers[i] << '\n';
}


char **CGII::getENV()
{
	size_t hSize = _Headers.size() + 1;
	_ENV = new char*[hSize]();
	for (size_t i = 0; i < hSize; i++)
	{
		size_t size = _Headers[i].length();
		_ENV[i] = new char[size + 1]();
		std::memmove(_ENV[i], _Headers[i].c_str(), size);
		// mstrcpy(_ENV[i], _Headers[i].c_str());
	}
	_Headers.clear();
	return _ENV;
}



Response *CGII::cgiError(int code)
{
	return errorRespo.getResponse(_Rq.com_srv_index, code, _Rq.host_name, _Rq.keepAlive);
}
