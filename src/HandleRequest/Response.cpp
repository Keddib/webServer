#include "Response.hpp"

Response::Response()
: _bSize(0), _keepAlive(1)
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

void Response::setHeader(const std::vector<std::string> &headers) // add bunch of header
{
	size_t s = headers.size();
	for (size_t i = 0; i < s; i++)
	_buffer += headers[i];
}

void Response::addBodyToBuffer(const std::string &text)
{
	_buffer += text;
}

void Response::setBodyfile(const std::string &file)
{
	_body.open(file, std::fstream::in | std::fstream::binary);
}

void Response::setKeepAlive(bool connection)
{
	_keepAlive = connection;
}


void Response::setBodySize(int64_t bsize)
{
	_bSize = bsize;
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

void Response::display()
{
	std::cout << "Response : \n";
	std::cout << "keep_alive: " << isKeepAlive() << '\n';
	std::cout << getBuffer();
	std::cout << _bSize << '\n';
}
