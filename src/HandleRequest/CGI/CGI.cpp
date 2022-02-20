	#include "CGI.hpp"


// var_export($_SERVER)

CGII::CGII(const Request &req, const ReqInfo &Rq)
:
_req(req),
_Loc(ServI[req.getCommonServerIndex()].whichServer(req.getHost()).whichLocation(req.getResource())) ,
_ENV(NULL),
_Rq(Rq)
{
	bodySize = 0;
	_Headers.reserve(13);
	CGItimeOut = _Loc.getCGItimeOut();
	_args[0] = (char *)_Loc.getCGIpath().c_str();
	_args[2] = NULL;
	_CGIfile = std::string("/tmp/.") + to_string((uintptr_t)this);

	_Content_Type_set = false;
}

CGII::~CGII()
{
	int i = 0;
	while (_ENV[i])
	{
		delete[] _ENV[i];
		++i;
	}
	delete [] _ENV;
}

void CGII::setENV()
{
	/*
	'CONTENT_LENGTH' => '',
  	'CONTENT_TYPE' => '',
  	'REQUEST_METHOD' => 'GET',
  	'QUERY_STRING' => 'php=1',
  	'FCGI_ROLE' => 'RESPONDER',
  	'PHP_SELF' => '/envs.php',
	*/
	std::string resource = _req.getResource();
	_Headers.push_back("REQUEST_URI=" + resource);
	std::string QS = getQueryString(resource); // this func remove the QS from resource
	_Headers.push_back("DOCUMENT_URI=" + resource);
	_Headers.push_back("SCRIPT_NAME=" + resource);
	// _Headers.push_back("PATH_INFO=" + resource);
	if (resource[resource.size() - 1] == '/') // is directory there is a problem here
		resource += _Rq.CGIfile;
	_PATH = _Loc.getRoute() + resource;
	_args[1] = (char *)_PATH.c_str();
	_Headers.push_back("SCRIPT_FILENAME=" + _PATH);
	_Headers.push_back("PATH_TRANSLATED=" + _PATH);
	_Headers.push_back("QUERY_STRING=" + QS);
	_Headers.push_back("SERVER_NAME=" + _req.getHost());
	_Headers.push_back("REQUEST_METHOD=" + get_method(_req.getMethod()));
	_Headers.push_back("GETAWAY_INTERFACE=CGI/1.1");
	_Headers.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_Headers.push_back("REDIRECT_STATUS=200");
	_Headers.push_back("FCGI_ROLE=RESPONDER");
	_Headers.push_back("REQUEST_SCHEME=http");
	_Headers.push_back("DOCUMENT_ROOT=" + _Loc.getRoute());

	_Headers.push_back("SERVER_SOFTWARE=webserv/1.1 " + getOsName());
	int Port = ServI[_req.getCommonServerIndex()].GetIpPortCommonServer().second;
	_Headers.push_back("SERVER_PORT=" + to_string(Port));
	// remote client info
	const std::pair<std::string, int> &remoteInfo = _req.GetClientInfo();
	_Headers.push_back("REMOTE_ADDR=" + remoteInfo.first);
	_Headers.push_back("REMOTE_PORT=" + to_string(remoteInfo.second));
	if(const char* env_p = std::getenv("PATH"))
		_Headers.push_back("PATH=" + std::string(env_p));
	// need only content-type and content-lenght
	_Headers.push_back("CONTENT_LENGTH=" + to_string(_req.getBodySize()));
	addRequestHeaders();
	// {
	// 	size_t size = _Headers.size();
	// 	for (size_t i = 0; i < size; i++)
	// 	{
	// 		std::cout << _Headers[i] << '\n';
	// 	}
	// }
	if (_Content_Type_set == false)
		_Headers.push_back("CONTENT_TYPE=");
	getENV();
	int i = 0;
	while (_ENV[i])
		std::cout << _ENV[i++] << '\n';
}

int CGII::excuteChildProcess(int inFD, int outFD)
{
	DONE = false;
	c_pid = fork();
	if (c_pid < 0)
		return (-1);
	_sTime = std::time(NULL);
	// if the pid is zero, this is the child process
	if (c_pid == 0) // child
	{
		std::cerr << "inside child\n";
		// change directory to the the scripte directory
		chdir(_PATH.substr(0, _PATH.find_last_of('/')).c_str());

		// Child. Start by closing descriptors we
		//  don't need in this process
		if (dup2(outFD, 1) == -1)
			exit(500);
		if (dup2(inFD, 0) == -1)
			exit(500);
		if (close(inFD) || close(outFD))
			exit(500);
		// fcntl(1, F_SETFL, O_NONBLOCK);
		// fcntl(0, F_SETFL, O_NONBLOCK);
		execve(_args[0], _args, _ENV);
		exit(502);
	}
	return 0;
}

Response *CGII::getResponse()
{
	std::cout << "get respo cgi \n";
	int status;
	// int Rfd[2]; // pipe for parent to read child output
	// int Wfd[2]; // pipe for child to read parent output
	// if (pipe(Rfd) < 0 || pipe(Wfd) < 0)
	// 	return cgiError(500);
	int inFD = 0;
	if (_Rq.meth == POST) {
		inFD = open(_req.getFileName(), O_RDONLY);
		if (inFD == -1)
		{
			std::cout << "failed\n";
			return cgiError(500);
		}
	}
	int outFD = open(_CGIfile.c_str(), O_RDWR | O_CREAT, 0666);
	if (outFD == -1)
	{
		std::cout << "out not created\n";
		return cgiError(500);
	}
	if (excuteChildProcess(inFD, outFD))
		return cgiError(500);
	if (inFD != 0)
		if (close(inFD))
			return cgiError(500);
	if (close(outFD))
		return cgiError(500);
	// check timeout
	while (std::time(NULL) - _sTime < CGItimeOut && !DONE)
		usleep(5000);
	if (!DONE) // if time ended and child didn't end yet return timeout
	{
		kill(c_pid, SIGKILL);
		return cgiError(504);
	}
	waitpid(c_pid, &status , 0); // wait for child status code
	status = WEXITSTATUS(status);
	if ( status == 500 ||  status == 502) // if error return error
		return cgiError(status);
	// parse the CGI response
	return ParseCGIresponse(_CGIfile);
}

/*
int hundleCGIheader(std::string &Line)
{

(CASE1)	content-type && (optional status && !location) -> Document Response: (body)
		-> mains that we need to add connection header
		-> content-lenght
		-> return the response;

(CASE2)	content-type && location (!status) with $scheme(http(s)://) -> Client Redirect Response (may body)
		-> mains that we need to add connection header
		-> add status code 302
		-> return the response;

(CASE3)	content-type && location (!status) with local PATH -> Local Redirect Response (no body)
		-> look for the file in the location and return it with 200 ok
		-> add header
		-> return reponse;

(CASE4)	content-type and status and location -> Client Redirect Response with Document: (body)
		-> mains that we need to add connection header
		-> content-lenght
		-> return the response;

	return 1;
 }
*/
// remove this overload
// std::ostream&	operator<<(std::ostream &os, std::vector<std::string> &v)
// {
// 	for (auto &str : v)
// 		os << str << "\n";
// 	return os;
// }

Response	*CGII::DocumentResponse()
{
	Response *res = new Response();
	short code = _cgii_res_info.status.second;
	res->setCommonServerIndex(_Rq.com_srv_index);
	res->setStartLine("HTTP/1.1", code, getErrorMessage(code));
	if (!_Rq.keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	else
		res->setHeader("Connection", "keep-alive");
	res->setHeader(_CGIheaders);
	unsigned int bsize = getFileSize(_CGIfile.c_str());
	bsize -= bodySize;
	res->setHeader("Content-Length", to_string(bsize), 1);
	res->setBodySize(bsize);
	res->setBodyfile(_CGIfile, bodySize);
	res->display();
	return res;
}

Response	*CGII::ClientRedirectResponse()
{
	Response *res = new Response();
	short code = _cgii_res_info.status.second;
	res->setCommonServerIndex(_Rq.com_srv_index);
	res->setStartLine("HTTP/1.1", code, getErrorMessage(code));
	if (!_Rq.keepAlive)
	{
		res->setHeader("Connection", "close");
		res->setKeepAlive(false);
	}
	else
		res->setHeader("Connection", "keep-alive");
	res->setHeader(_CGIheaders);
	if (_cgii_res_info.bodyExist)
	{
		unsigned int bsize = getFileSize(_CGIfile.c_str());
		bsize -= bodySize;
		res->setHeader("Content-Length", to_string(bsize), 1);
		res->setBodySize(bsize);
		res->setBodyfile(_CGIfile, bodySize);
	} else {
		//set defualt defualt redirect page
		std::string redirectPage;
		getErrorPage(code, redirectPage);
		size_t lenght = std::strlen(redirectPage.c_str());
		// add content-lenght header ;
		res->setHeader("Content-Length", to_string(lenght), 1);
		res->addBodyToBuffer(redirectPage);
	}
	res->display();
	return res;
}

Response	*CGII::ResponseConstruction()
{
	if (_cgii_res_info.location)
	{
		if (!_cgii_res_info.status.first)
			_cgii_res_info.status.second = 302; // default redirect status
		return ClientRedirectResponse();
	}
	else
		return DocumentResponse();
	return NULL;
}

bool	my_strncmp(const char *s1, const char *s2, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		if (tolower(s1[i]) != tolower(s2[i]))
			return false;
	return true;
}

Response *CGII::ParseCGIresponse(const std::string &CGIfileRespone)
{
	_CGIres.open(_CGIfile, std::fstream::in | std::fstream::binary);
	int delm;
	int index = 0;
	char *str;
	if (_CGIres.is_open())
	{
		do {
			_CGIres.getline(_buff, BUFFER_SIZE);
			if (_CGIres.gcount() == 0)
				return cgiError(502);
			bodySize += _CGIres.gcount();
			if (_buff[0] == '\r' || _buff[0] == '\0')
				break ; // means that empty lines after header fields
		//	if (_buff[0] == 'X')
		//		continue ;
			delm = IndexOf(_buff, ':');
			//if (delm == -1)
				//means that header is not in good syntax
			if (!_cgii_res_info.cont_type && my_strncmp(_buff, "Content-Type", delm))
				_cgii_res_info.cont_type = true;
			else if (!_cgii_res_info.location && my_strncmp(_buff, "Location", delm))
			{
				++delm;
				while (_buff[delm] == ' ')
					++delm;
				_cgii_res_info.loc_info.first = index;
				_cgii_res_info.loc_info.second = delm;
				_cgii_res_info.location = true;
			}
			else if (!_cgii_res_info.status.first && my_strncmp(_buff, "Status", delm)) // myabe the status shuold be lower case
			{
				// if buf = Status: 200
				// buf + index + 1  will be " 200"
				str = _buff + delm + 1;
				while (*str == ' ')
					++str;
				_cgii_res_info.status.first = true; // Status: 302 Found
				_cgii_res_info.status.second = atoi(str); // if i need to take at as number write atoi(str)
				continue ;
			}
			_CGIheaders.push_back(_buff);
			_CGIheaders.back() += '\n';
			++index;
		}
		while (_buff[0]); // it will break bec of empty line after headers
		// after this loop file pointer will be at body if there's one
		_CGIres.read(_buff, 10);
		_cgii_res_info.bodyExist = _CGIres.gcount();
		_CGIres.close();
		return ResponseConstruction();
	}
	else
		return errorRespo.getResponse(_Rq.com_srv_index, 500, _Rq.host_name, _Rq.keepAlive);
}

void putCharInFile(std::fstream &f, char *buff, size_t size)
{
		for (size_t i = 0; i < size; ++i)
			f << buff[i];
}

// int CGII::ReadCGIresponse(int fd) // rethink of this
// {
// 	// read data and store it on file
// 	_CGIres.open(_CGIfile, std::fstream::out | std::fstream::binary);
// 	int read_data;
// 	do {
// 		read_data = read(fd, _buff, BUFFER_SIZE);
// 		if (read_data == -1)
// 			return -1;
// 		_buff[read_data] = 0;
// 		putCharInFile(_CGIres, _buff, read_data);
// 	}while(read_data);
// 	_CGIres.close();
// 	return 0;
// }

// int CGII::SendFile(int fd)
// {
// 	std::fstream &file= const_cast<std::fstream &>(_req.getBodyFile());
// 	file.seekg(0);
// 	std::streamsize read_data;
// 	do {
// 		file.read(_buff, BUFFER_SIZE);
// 		read_data = file.gcount();
// 		if (read_data > BUFFER_SIZE)
// 			break ; // possiblae error
// 		_buff[read_data] = 0;
// 		if (write(fd, _buff, read_data) == -1)
// 			return -1;
// 	}while(read_data);
// 	return 0;
// }

void CGII::addRequestHeaders()
{
	size_t size = _req.aHeaders.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_req.aHeaders[i].first == "Content-Type")
		{
			_Content_Type_set = true;
			_Headers.push_back("CONTENT_TYPE=" + _req.aHeaders[i].second);
		}
		else if (isHeaderServerSpecific(_req.aHeaders[i].first))
			continue;
		else
			trasferReqHeaderToCGIheader(_req.aHeaders[i]); // trasfer request header to CGI header and add it as an env variable
	}
}


void CGII::trasferReqHeaderToCGIheader(const std::pair<std::string, std::string> &Hfield)
{
	// transfer Hfeild name to UPPERCASE_ and add prefix HTTP_
	std::cout << "HTTP_h\n";
	std::string FieldName("HTTP_");
	FieldName += Hfield.first;
	std::transform(FieldName.begin(), FieldName.end(), FieldName.begin(), asciiToUpper);
	FieldName.push_back('=');
	_Headers.push_back(FieldName + Hfield.second);
}

bool CGII::isHeaderServerSpecific(const std::string &hName) const
{
	if (hName == "Authorization" || hName == "Content-Length")
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



char **CGII::getENV()
{
	size_t hSize = _Headers.size() + 1;
	_ENV = new char* [hSize];
	for (size_t i = 0; i < hSize - 1; i++)
	{
		size_t size = _Headers[i].length();
		_ENV[i] = new char[size + 1];
		std::memmove(_ENV[i], _Headers[i].c_str(), size + 1);
		// mstrcpy(_ENV[i], _Headers[i].c_str());
	}
	_ENV[hSize - 1] = NULL;
	return _ENV; // maybe this return not needed since this var is in this obj
}



Response *CGII::cgiError(int code)
{
	return errorRespo.getResponse(_Rq.com_srv_index, code, _Rq.host_name, _Rq.keepAlive);
}
