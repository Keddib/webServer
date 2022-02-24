#include "Response.hpp"


Response::Response()
: _bSize(0), _keepAlive(1), _isFileUsed(1), _isReady(1), _PID(-1)
{

}

Response::~Response()
{
	if (_bodyFileName.size()) {
		std::cout << "\033[31m removed: " << _bodyFileName << "\033[0m\n";
		remove(_bodyFileName.c_str());
	}
	if (_PID != -1)
		kill(_PID, SIGKILL);
}

void Response::setStartLine(
	const std::string &scheme,
	int code,
	const std::string &message)
{
	_buffer += scheme + " " + to_string(code) + " " + message + "\r\n";
	// add to headers that are valid for all responses
	// Date and Server
	setHeader("Server", std::string("webserv/1.1 ") + getOsName(), 0);
	setHeader("Date", getDate(), 0);
	setHeader("Accept-Ranges", "none", 0);
}

void Response::setHeader(const std::string &key, const std::string &value, bool end)
{
	_buffer += key + ": " + value + "\r\n";
	if (end)
		_buffer += "\r\n";
}

void Response::setHeader(const std::vector<std::string> &headers) // add bunch of header
{
	size_t s = headers.size();
	for (size_t i = 0; i < s; i++)
	_buffer += headers[i];
}

void Response::setHeader(std::string header) // add bunch of header
{
	// std::cerr << header;
	this->_buffer += header;
}

void Response::addBodyToBuffer(const std::string &text)
{
	_buffer += text;
}


void Response::setHeaderSize(size_t size)
{
	_headersSize = size;
}

void Response::setBodyfile(const std::string &file)
{
	_body.open(file, std::fstream::in | std::fstream::binary);
}

void Response::setBodyfile(unsigned int size)
{
	_body.open(_bodyFileName, std::fstream::in | std::fstream::binary);
	_body.seekg(size);
}

void Response::setKeepAlive(bool connection)
{
	_keepAlive = connection;
}


void Response::setBodySize(int64_t bsize)
{
	_bSize = bsize;
}

void Response::setHost(const std::string &h)
{
	_host = h;
}


bool Response::isKeepAlive() const
{
	return _keepAlive;
}

unsigned int Response::getBodySize() const
{
	return _bSize;
}

const std::string& Response::getBuffer() const
{
	return _buffer;
}


// this is added
std::fstream &Response::getBody()
{
	return _body;
}


int	Response::getCommonServerIndex() const
{
	return cmSrvFd;
}

void	Response::setCommonServerIndex(int s)
{
	cmSrvFd = s;
}

bool Response::isFileUsed() const
{
	return _isFileUsed;
}

void Response::setNotReady(time_t time, int PID, const std::string &file, bool ready)
{
	_startTime = time;
	_PID = PID;
	_isReady = ready;
	_bodyFileName = file;
}

const std::string &Response::getBodyFile() const
{
	return _bodyFileName;
}

void Response::setTimeout(time_t time)
{
	_cgiTO = time;
}

bool	Response::getResponse(int status)
{
	if ( status == 500 ||  status == 502) // if error return error
	{
		getErrorResponse(status);
		return true;
	}
	CGIIresInfo resInfo = ParseCGIresponse(this, _bodyFileName);
	if (resInfo.error)
	{
		getErrorResponse(resInfo.error);
		return true;
	}
	if (resInfo.location)
	{
		if (!resInfo.status.first)
			resInfo.status.second = 302; // default redirect status
		ClientRedirectResponse(resInfo);
	}
	else
		DocumentResponse(resInfo.status.second);
	return true;
}


void	Response::DocumentResponse( int code )
{
	_buffer.insert(0, "Accept-Ranges: none\r\n");
	_buffer.insert(0, "Date: " + getDate() + "\r\n");
	_buffer.insert(0, "Server: webserv/1.1 " + getOsName() + "\r\n");
	_buffer.insert(0, "HTTP/1.1 " + to_string(code) + " " + getErrorMessage(code) + "\r\n");
	if (!_keepAlive)
	{
		setHeader("Connection", "close");
		setKeepAlive(false);
	}
	else
		setHeader("Connection", "keep-alive");
	_bSize = getFileSize(_bodyFileName.c_str());
	_bSize -= _headersSize;
	setHeader("Content-Length", to_string(_bSize), 1);
	setBodyfile(_headersSize);
}

void	Response::ClientRedirectResponse(CGIIresInfo &resInfo)
{
	_buffer.insert(0, "Accept-Ranges: none\r\n");
	_buffer.insert(0, "Date: " + getDate() + "\r\n");
	_buffer.insert(0, "Server: webserv/1.1 " + getOsName() + "\r\n");
	_buffer.insert(0, "HTTP/1.1 " + to_string(resInfo.status.second) + " " + getErrorMessage(resInfo.status.second) + "\r\n");
	if (!_keepAlive)
	{
		setHeader("Connection", "close");
		setKeepAlive(false);
	}
	else
		setHeader("Connection", "keep-alive");
	if (resInfo.bodyExist)
	{
		_bSize = getFileSize(_bodyFileName.c_str());
		_bSize -= _headersSize;
		setHeader("Content-Length", to_string(_bSize), 1);
		setBodyfile(_bSize);
	} else {
		std::string redirectPage;
		getErrorPage(resInfo.status.second, redirectPage);
		size_t length = redirectPage.size();
		// add content-lenght header ;
		setHeader("Content-Length", to_string(length), 1);
		addBodyToBuffer(redirectPage);
	}
}

bool	Response::isReady()
{
	if (_isReady)
		return true;
	int status;
	if (std::time(NULL) - _startTime < _cgiTO)
	{
		if (waitpid(_PID, &status, WNOHANG))
		{
			_PID = -1;
			std::cout << "child is done\n";
			_isReady = true;
			return getResponse(WEXITSTATUS(status));
		}
		else
			return false;
	}
	kill(_PID, SIGKILL);
	_PID = -1;
	getErrorResponse(504);
	_isReady = true;
	return true;
}





void	Response::getErrorResponse(int error)
{
	setStartLine("HTTP/1.1", error, getErrorMessage(error));
	if (!_keepAlive)
	{
		setHeader("Connection", "close");
		setKeepAlive(false);
	}
	else
		setHeader("Connection", "keep-alive");

	if (setUserErrorPage(error))
		return;
	else { // if not we use our default one
		std::string errorPage;
		getErrorPage(error, errorPage);
		size_t lenght = errorPage.size();
		setHeader("Content-Type", "text/html");
		setHeader("Content-Length", to_string(lenght), 1);
		addBodyToBuffer(errorPage);
	}
}

bool Response::setUserErrorPage(int error)
{
	std::string errorPagePath;
	const std::vector<std::pair<int, std::string> > &Errors =
	ServI[cmSrvFd].whichServer(_host).getErrorPages();
	// loop over error pages and check if the server has error pages
	for(size_t i=0; i < Errors.size(); i++)
	{
		if (Errors[i].first == error)
			errorPagePath = Errors[i].second;
	}
	FileInfo fData;
	// we check if a error page path is correct we use it as response body
	if (!errorPagePath.empty() && (getFileInfo(errorPagePath, fData) == 0))
	{
		// check file type and add content-type header
		setHeader("Content-Type", fData.Ftype);
		// check file size and add content-lenght header
		setHeader("Content-Length", to_string(fData.size));
		setBodySize(fData.size);
		setHeader("Last-Modified", fData.Mtime, 1);
		// add file to body of request;
		setBodyfile(errorPagePath);
		return true;
	}
	return false;
}
















void Response::display()
{
	std::cout << "Response : \n";
	std::cout << "keep_alive: " << isKeepAlive() << '\n';
	std::cout << getBuffer();
	std::cout << _bSize << '\n';
}
