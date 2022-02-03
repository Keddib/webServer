#include "Response.hpp"

Response::Response()
: _bodyFD(-1), _bSize(0), _keepAlive(1)
{
}

Response::~Response()
{
	if (_bodyFD > -1)
		close(_bodyFD);
}

void Response::setStartLine(
	const std::string &scheme,
	int code,
	const std::string &message)
{
	_header += scheme + " " + std::to_string(code) + " " + message + "\r\n";
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	_header += key + ": " + value + "\r\n";
}

void Response::setBodyFD(int fd)
{
	_bodyFD = fd;
}

bool Response::isKeepAlive()
{
	return _keepAlive;
}

int Response::getBodyFD()
{
	return _bodyFD;
}

unsigned int Response::getBodySize()
{
	return _bSize;
}
const std::string& Response::getHeader()
{
	return _header;
}
