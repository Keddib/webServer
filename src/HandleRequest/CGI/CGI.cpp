#include "CGI.hpp"



CGII::CGII(const Request &req, const ReqInfo &Rq)
:
_req(req),
_Loc(ServI[req.getCommonServerIndex()].whichServer(req.getHost()).whichLocation(req.getResource())) ,
_ENV(NULL),
_Rq(Rq)
{
	_Headers.reserve(13);
	CGItimeOut = _Loc.getCGItimeOut();

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
	std::cout << "before getENV\n";
	getENV();
	std::cout << "after getENV\n";
}

int CGII::excuteChildProcess(int Rfd[], int Wfd[])
{
	std::cout << "inside excute child\n";
	DONE = true;
	c_pid = fork();
	if (c_pid < 0)
		return (-1);
	_sTime = std::time(NULL);
	// if the pid is zero, this is the child process
	if (c_pid == 0) // child
	{
		// Child. Start by closing descriptors we
		//  don't need in this process
		if (close(Rfd[READ]) || close(Wfd[WRITE]))
			exit(500);
		if (dup2(Rfd[WRITE], 1) == -1 || dup2(Wfd[READ], 0))
			exit(500);
		if (close(Wfd[READ]) || close(Rfd[WRITE]))
			exit(500);
		// execve(CGI, script, _ENV);
		exit(502);
	}
	// // Parent. close unneeded descriptors
	close(Rfd[WRITE]);
	close(Wfd[READ]);
	return 0;
}

Response *CGII::getResponse()
{
	std::cout << "inside get respo\n";
	int status;
	int Rfd[2]; // pipe for parent to read child output
	int Wfd[2]; // pipe for child to read parent output
	if (pipe(Rfd) < 0 || pipe(Wfd) < 0)
		return cgiError(500);
	std::cout << "before child\n";
	if (excuteChildProcess(Rfd, Wfd))
		return cgiError(500);
	std::cout << "line 95\n";
	// if request is post write body to Wfd[write], otherwise close the writing end of pipe
	// send a value to the child
	if (_Rq.meth == POST)
	{
		if (_req.isBodyStr())
		{
			int wRet = write(Wfd[WRITE], _req.getBody().c_str(), _req.getBody().size());
			if (wRet < 0)
				return cgiError(500);
		}
		else
		{
			if (SendFile(Wfd[WRITE]) == -1)
				return cgiError(500);
			std::cout << "line send file 107\n";
		}
	}
	close(Wfd[WRITE]);
	// check timeout
	while (std::time(NULL) - _sTime < CGItimeOut && !DONE)
		usleep(500);
	if (!DONE) // if time ended and child didn't end yet return timeout
	{
		kill(c_pid, SIGKILL);
		return cgiError(504);
	}
	waitpid(c_pid, &status , 0); // wait for child status code
	status = WEXITSTATUS(status);
	if ( status == 500 ||  status == 502) // if error return error
	{
		std::cout << "status = " << status << '\n';
		return cgiError(status);
	}
	else // else read response and parse it
	{
		if (ReadCGIresponse(Wfd[READ]) == -1)
			return cgiError(500);
	}
	close(Rfd[READ]); // close read end of pipe
	//
	exit(1);
	// else if (len == 0) if no output is supplied check exit status code of child
	// parse the CGI response
	return NULL; // return ParseCGIresponse(buffer);
}

void putCharInFile(std::fstream &f, char *buff, size_t size)
{
		for (size_t i = 0; i < size; ++i)
			f << buff[i];
}

int CGII::ReadCGIresponse(int fd)
{
	std::cout << "reead CGI RES\n";
	// read data and store it on file
	_CGIres.open("/tmp/.CGIres", std::fstream::out | std::fstream::in | std::fstream::binary);
	std::cout << "is open : "<< _CGIres.is_open() << "\n";
	int read_data;
	do {
		read_data = read(fd, _buff, BUFFER_SIZE);
		if (read_data == -1)
		{
			std::cout << "read failed\n";
			return -1;
		}
		_buff[read_data] = 0;
		std::cout << _buff << '\n';
		putCharInFile(_CGIres, _buff, read_data);
	}while(read_data);
	_CGIres.close();
	return 0;
}

int CGII::SendFile(int fd)
{
	std::fstream &file= const_cast<std::fstream &>(_req.getBodyFile());
	std::streamsize read_data;
	do {
		file.read(_buff, BUFFER_SIZE);
		read_data = file.gcount();
		if (read_data > BUFFER_SIZE)
			break ;
		_buff[read_data] = 0;
		if (write(fd, _buff, read_data) == -1)
			return -1;
	}while(read_data);
	return 0;
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
	_ENV = new char*[hSize];
	for (size_t i = 0; i < hSize - 1; i++)
	{
		size_t size = _Headers[i].length();
		_ENV[i] = new char[size + 1]();
		std::memmove(_ENV[i], _Headers[i].c_str(), size + 1);
		// mstrcpy(_ENV[i], _Headers[i].c_str());
	}
	_ENV[hSize - 2] = NULL;
	_Headers.clear();
	return _ENV;
}



Response *CGII::cgiError(int code)
{
	return errorRespo.getResponse(_Rq.com_srv_index, code, _Rq.host_name, _Rq.keepAlive);
}
