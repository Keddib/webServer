#include "Request.hpp"

Request::Request(int cmn, int meth, const std::string &res, bool ver)
: _resource(res),_commonServ(cmn), _method(meth), _version(ver)
{
}

Request::~Request()
{
	// if (_bodyFD > -1)
	// 	close(_bodyFD);
}

int Request::getMethod() const
{
	return _method;
}

const std::string& Request::getResource() const
{
	return _resource;
}


bool Request::whichVersion() const
{
	return _version;
}

const std::string& Request::getHostName() const
{
	return _hostName;
}

size_t Request::getCSindex() const
{
	return _commonServ;
}
