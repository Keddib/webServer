#include "Response.hpp"

Response::Response()
: _bSize(0), _keepAlive(1), _bodyExcite(0)
{

}

Response::~Response()
{
}

void Response::setStartLine(
	const std::string &scheme,
	int code,
	const std::string &message)
{
	_buffer += scheme + " " + std::to_string(code) + " " + message + "\r\n";
}

void Response::setHeader(const std::string &key, const std::string &value)
{
	_buffer += key + ": " + value + "\r\n";
}


bool Response::isKeepAlive()
{
	return _keepAlive;
}


unsigned int Response::getBodySize()
{
	return _bSize;
}
const std::string& Response::getBuffer()
{
	return _buffer;
}

void Response::setBodyfile(const std::string &file)
{
	_body.open(file, std::fstream::in | std::fstream::binary);
	_bodyExcite = 1;
	_bSize = 10; // need to get the size of file
}
