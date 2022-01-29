#include "Location.hpp"

Location::Location( const std::string &prefix)
: _prefix(prefix), _autoIndex(0)
{
}

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
	_CGI = CGIpath;
}

void Location::setUpload( const std::string &UPpath )
{
	_upload = UPpath;
}

const std::string& Location::getPrefix() const
{
	return _prefix;
}

void Location::Display() const
{
	std::cout << "Location: \n" <<
	"\tPrefix : " << _prefix + "\n" <<
	"\tRoot : " << _root + "\n" <<
	"\tCGI : " << _CGI + "\n";
	std::cout << "\tRet : " << std::to_string(_ret.first) + " " << _ret.second + "\n";
	std::cout << "\tIndex: ";
	for (size_t i = 0; i < _index.size(); i++)
		std::cout << _index[i] << " | ";
	std::cout << "\n";
	std::cout << "\tMethods: ";
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << _methods[i] << " | ";
	std::cout << "\n";
	std::cout << "\tauto index : " << std::to_string(_autoIndex) + "\n";

}
