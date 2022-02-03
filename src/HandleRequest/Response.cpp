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
	// add to headers that are valid for all responses
	// Date and Server
	setHeader("Server", std::string("webserv/1.1 ") + getOsName(), 0);
	setHeader("Date", getDate(), 0);
}

void Response::setHeader(const std::string &key, const std::string &value, bool end)
{
	_buffer += key + ": " + value + "\r\n";
	if (end)
		_buffer += "\r\n";
}

void Response::addBodyToBuffer(const std::string &text)
{
	_buffer += text;
}

void Response::setBodyfile(const std::string &file)
{
	_body.open(file, std::fstream::in | std::fstream::binary);
	_bodyExcite = 1;
	_bSize = 10; // need to get the size of file
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

void Response::display() const
{
	std::cout << "Response : \n";
	std::cout << "keep_alive: " << isKeepAlive() << '\n';
	std::cout << getBuffer();
}