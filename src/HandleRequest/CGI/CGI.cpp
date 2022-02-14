#include "CGI.hpp"



CGII::CGII(const Request &req)
: _req(req),
_Loc(ServI[req.getCommonServerIndex()].whichServer(req.getHost()).whichLocation(req.getResource())) ,
_ENV(NULL)
{
	// _Headers.reserve(13);

}

CGII::~CGII()
{
	// size_t size = _Headers.size();
	// for (int i = 0; i < size; i++)
	// 	if (_ENV[i])
	// 		delete _ENV[i];
	// delete _ENV;
}

char **CGII::getENV()
{
	size_t hSize = _Headers.size();
	_ENV = new char*[hSize]();
	for (size_t i = 0; i < hSize; i++)
	{
		_ENV[i] = new char[_Headers[i].size()];
		strcpy(_ENV[i], _Headers[i].c_str());
	}
	return _ENV;
}

void CGII::setENV()
{
	std::cout << "inside setENV\n";
	std::string resource = _req.getResource();

	_Headers.push_back("REQUEST_URI=" + resource);
	std::string QS = getQueryString(resource); // this func remove the QS from resource
	std::string PATH = _Loc.getRoute() + resource;
	_Headers.push_back("SCRIPT_NAME=" + resource);
	_Headers.push_back("PATH_INFO=" + resource);
	_Headers.push_back("SCRIPT_FILENAME=" + PATH);
	_Headers.push_back("PATH_TRANSLATED=" + PATH);
	_Headers.push_back("QUERY_STRING=" + QS);
	_Headers.push_back("SERVER_NAME=" + _req.getHost());
	_Headers.push_back("REQUEST_METHOD=" + get_method(_req.getMethod()));
	_Headers.push_back("GETAWAY_INTERFACE=CGI/1.1");
	_Headers.push_back("SERVER_PROTOCOL=HTTP/1.1");
	_Headers.push_back("SERVER_SOFTWARE=webserv/1.1 " + getOsName());
	int Port = ServI[_req.getCommonServerIndex()].GetIpPortCommonServer().second;
	_Headers.push_back("SERVER_PORT=" + std::to_string(Port));
	// remote client info
	const std::pair<std::string, int> &remoteInfo = _req.GetClientInfo();
	_Headers.push_back("REMOTE_ADDR=" + remoteInfo.first);
	// _Headers.push_back("REMOTE_PORT=" + std::to_string(remoteInfo.second));
	if(const char* env_p = std::getenv("PATH"))
		_Headers.push_back("PATH=" + std::string(env_p));
	// need only content-type and content-lenght
	if (_req.getMethod() == POST)
		_Headers.push_back("CONTENT_LENGTH=" + std::to_string(_req.getBodySize()));
	addRequestHeaders();
}

void CGII::addRequestHeaders()
{
	size_t size = _req.aHeaders.size();
	for (size_t i = 0; i < size; i++)
	{
		if (_req.aHeaders[i].first == "Content-Type")
			_Headers.push_back("CONTENT_TYPE=" + _req.aHeaders[i].second);
		else if (isHeaderServerSpecific(_req.aHeaders[i].first))
			continue;
		else
			trasferReqHeaderToCGIheader(_req.aHeaders[i]); // trasfer request header to CGI header and add it as an env variable
	}
}


void CGII::trasferReqHeaderToCGIheader(const std::pair<std::string, std::string> &Hfield)
{
	// transfer Hfeild name to UPPERCASE_ and add prefix HTTP_
	std::string FieldName; // trasfered
	std::cout << Hfield.first << ' ' << Hfield.second << '\n';
	_Headers.push_back(FieldName + Hfield.second);
}

bool CGII::isHeaderServerSpecific(const std::string &hName) const
{
	if (hName == "Authorization" || hName == "Content-Length" || hName == "Connection")
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


void CGII::display() const
{
	std::cout << "inside display\n";
	size_t s = _Headers.size();
	std::cout << "size of header seted in CGI " << s << '\n';
	for (size_t i = 0; i < s; i++)
		std::cout << _Headers[i] << '\n';
}
