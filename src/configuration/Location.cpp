#include "Location.hpp"

Location::Location( const std::string &prefix)
: _prefix(prefix), _autoIndex(0), _root(DEFAULT_ROOT), _cgiTimeOut(60)
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

void Location::setMethods( int method )
{
	std::vector<int>::const_iterator start, end;
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

void Location::setCGI( const std::string &CGIext,  const std::string &CGIpath)
{
	_CGIext = CGIext;
	_CGI = CGIpath;
}

void Location::setUpload( const std::string &UPpath )
{
	_upload = UPpath;
}

void Location::setCGItimeOut( unsigned int time)
{
	_cgiTimeOut = time;
}

void Location::fillAllowedMethods(std::string &allow) const
{
	size_t s = _methods.size();
	char p[2] = { ',', ' ' };
	for(size_t i =0; i < s; i++)
	{
		if (i+1 == s)
			p[0] = '\0';
		if (_methods[i] == POST)
			allow += std::string(POST_STR) + p;
		else if (_methods[i] == GET)
			allow += std::string(GET_STR) + p;
		else if (_methods[i] == DELETE)
			allow += std::string(DELETE_STR) + p;
	}

}

const std::string& Location::getPrefix() const
{
	return _prefix;
}


bool Location::isMethodAllowed(int meth) const
{
	size_t size = _methods.size();
	for (size_t i = 0; i < size; i++)
		if (meth == _methods[i])
			return true;
	return (!size && meth != DELETE);
}

const std::string& Location::getRoute() const
{
	return _root;
}

const std::vector<std::string>& Location::getIndexes() const
{
	return _index;
}

bool Location::isAutoIndexOn() const
{
	return _autoIndex;
}

bool Location::isRedirect() const
{
	return _ret.first;
}

int Location::getRedirectCode() const
{
	return _ret.first;
}

const std::string& Location::getRedirectURI() const
{
	return _ret.second;
}

const std::string& Location::getCGIext() const
{
	return _CGIext;
}

const std::string& Location::getCGIpath() const
{
	return _CGI;
}

bool Location::isCGI() const
{
	return !_CGI.empty();
}

bool Location::isUPLOAD() const
{
	return !_upload.empty();
}

unsigned int Location::getCGItimeOut() const
{
	return _cgiTimeOut;
}

const std::string& Location::getUPLOADpath() const
{
	return _upload;
}

void Location::Display() const
{
	std::cout << "Location: \n" <<
	"\tPrefix : " << _prefix + "\n" <<
	"\tRoot : " << _root + "\n" <<
	"\tCGI : " << _CGI + "\n";
	std::cout << "\tRet : " << to_string(_ret.first) + " " << _ret.second + "\n";
	std::cout << "\tIndex: ";
	for (size_t i = 0; i < _index.size(); i++)
		std::cout << _index[i] << " | ";
	std::cout << "\n";
	std::cout << "\tMethods: ";
	for (size_t i = 0; i < _methods.size(); i++)
		std::cout << _methods[i] << " | ";
	std::cout << "\n";
	std::cout << "\tauto index : " << to_string(_autoIndex) + "\n";

}
