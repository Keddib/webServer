#include "Location.hpp"

Location::Location( std::string prefix) : _prefix(prefix), _autoIndex(0) {}

Location::~Location() {}

// getters

void Location::setRoot( const std::string &path )
{
	_root = path;
}

void Location::setAutoIndex( bool x )
{
	_autoIndex = x;
}

void Location::setReturn( int code, const std::string &path )
{
	_ret.first = code;
	_ret.second = path;
}

void Location::setIndex( const std::string &index )
{
	std::vector<std::string>::const_iterator start, end;
	start = _index.begin();
	end = _index.end();
	while (start != end )
	{
		if (index == *start)
			return;
		++start;
	}
	_index.push_back(index);
}

void Location::setMethods( const std::string &method )
{
	std::vector<std::string>::const_iterator start, end;
	start = _methods.begin();
	end = _methods.end();
	while (start != end )
	{
		if (method == *start)
			return;
		++start;
	}
	_methods.push_back(method);
}

void Location::setCGI( const std::string &CGIpath )
{
	CGI = CGIpath;
}

std::ostream& operator<<(std::ostream &out, const Location &obj)
{
	return out;
}
