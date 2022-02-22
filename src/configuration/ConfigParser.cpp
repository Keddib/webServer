#include "ConfigParser.hpp"

ConfigParser::ConfigParser(std::string file)
: _cFile(file.c_str()), _lineNum(0), _insideServer(0), _insideLocation(0)
{
	if (!_cFile.is_open())
		throw std::string(
			std::string("ConfigFile: Couldn't open ")+ file + "\n"
		);
}

ConfigParser::~ConfigParser()
{
	_cFile.close();
}


void ConfigParser::parse()
{
	std::string line;
	while(getline(_cFile, line))
	{
		_lineNum++;
		removeSpacesFromStart(line);
		if(line.empty() || line[0] == '#')
			continue;
		removeSpacesFromEnd(line);
		if (line == "server")
		{
			_Vsrvs.push_back(std::pair<unsigned int, int>(0, D_PORT));
			_insideServer = 1;
			_insideLocation = 0;
			continue;
		}
		else if (_insideServer)
			serverBlock(line);
		else
			throwException(": inexpected token\n");
	}
	if (_Vsrvs.empty())
		_Vsrvs.push_back(std::pair<unsigned int, int>(0, D_PORT));
}

void ConfigParser::AddServersToServersInterface()
{
	for(size_t i = 0; i < _Vsrvs.size(); i++)
	{
		ServI.AddServer(_Vsrvs[i]);
	}
	_Vsrvs.clear();
}

void ConfigParser::serverBlock(std::string &line)
{
	// split line and check first word if a valid directive;
	std::vector<std::string> tokens;
	simpleTokenizer(line, tokens);
	int dir = whichDirective(tokens[0]);
	// if it's not valid or specific for location throw error
	if (dir == NONE)
		throwException(tokens[0] + "] unknown directive\n");
	// if it's a location add alocation add  a location object and go to next line
	else if (dir == LOCATION)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (prefix)\n");
		_Vsrvs.back().addLocation(tokens[1]);
		_insideLocation = 1;
		return ;
	}
	// if i's valid and a loction added passit to location to add it
	else if (_insideLocation)
	{
		if (dir < LOCATION)
			throwException(tokens[0] + " specific for server\n");
		// check which directive and add it to location
		addDirectiveToLocation(dir, tokens);
		return ;
	}
	else if (dir > LOCATION)
		throwException(tokens[0] + " is specific for loaction\n");
	// if valid and not spisefic for location add it to the last server on the vector
	else
		// check which directive and add it to server
		addDirectiveToServer(dir, tokens);
}

void ConfigParser::addDirectiveToLocation(int dir, std::vector<std::string> &tokens)
{
	if (dir == ROOT)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (path)\n");
		if (tokens[1][tokens[1].size() -1 ] == '/')
			tokens[1].erase(tokens[1].size() - 1);
		_Vsrvs.back().getLocationsToEdit().back().setRoot(tokens[1]);
	}
	else if (dir == AUTO)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (on/off)\n");
		if (tokens[1] == "on")
			_Vsrvs.back().getLocationsToEdit().back().setAutoIndex(1);
	}
	else if (dir == INDEX)
	{
		if (tokens.size() < 2)
			throwException(tokens[0] + " needs at least one value (exampl.html)\n");
		for(size_t i = 1; i < tokens.size(); i++)
			_Vsrvs.back().getLocationsToEdit().back().setIndex(tokens[i]);
	}
	else if (dir == ACCMETH)
	{
		if (tokens.size() < 2)
			throwException(tokens[0] + " needs at least one value (GET|POST|DELETE)\n");
		for(size_t i = 1; i < tokens.size(); i++)
		{
			int meth = isValidMethod(tokens[i]);
			if (meth)
				_Vsrvs.back().getLocationsToEdit().back().setMethods(meth);
			else
				std::cout << "WebServ/1.0: [warn] " << to_string(_lineNum) << " [" <<
					tokens[i] + "] not a supported method. ignored\n";
		}
	}
	else if (dir == CGI)
	{
		if (tokens.size() != 3)
			throwException(tokens[0] + " needs two values (extentions of CGI scripts and path to the interpreter)\n");
		if (_Vsrvs.back().getLocationsToEdit().back().isUPLOAD())
			throwException(tokens[0] + " Not expected (CGI and UPLOAD can't be on the same loaction)\n");

		_Vsrvs.back().getLocationsToEdit().back().setCGI(tokens[1], tokens[2]);

	}
	else if (dir == CGI_TO)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one values (time in seconds)\n");
		int time = atoi(tokens[1].c_str());
		if (time <= 0)
			throwException(tokens[1] + " invalid time out\n");
		_Vsrvs.back().getLocationsToEdit().back().setCGItimeOut(time);
	}
	else if (dir == UPLD)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (path to upload)\n");
		if (_Vsrvs.back().getLocationsToEdit().back().isCGI())
			throwException(tokens[0] + " Not expected (CGI and UPLOAD can't be on the same loaction)\n");
		_Vsrvs.back().getLocationsToEdit().back().setUpload(tokens[1]);
	}
	else if (dir == RET)
	{
		if (tokens.size() != 3)
			throwException(tokens[0] + " needs two values (redirect code and URI)\n");
		// check if redirect code valid
		int code = std::atoi(tokens[1].c_str());
		if (code > 308 || code < 300)
			throwException(tokens[1] + " it's not a valid redirect code (300-308)\n");
		_Vsrvs.back().getLocationsToEdit().back().setReturn(code, tokens[2]);
	}
}

void ConfigParser::addDirectiveToServer(int dir, std::vector<std::string> &tokens)
{
	if (dir == HOST)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (IPv4 address)\n");
		// check if valid host; conver to unsigned int
		_Vsrvs.back().setHostValue(tokens[1]);
	}
	else if (dir == PORT)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (Port)\n");
		// 1,024 – 49,151 registered ports.
		// check if valid port
		int port = std::atol(tokens[1].c_str());
		// if (port < 1024 || port > 49151)
			// throwException(tokens[0] + " Invalid port (1024–49151 registered ports only)\n");
		_Vsrvs.back().setPortValue(port);
	}
	else if (dir == S_NAME)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (server_name)\n");
		_Vsrvs.back().setServerNameValue(tokens[1]);
	}
	else if (dir == CMBS)
	{
		if (tokens.size() != 2)
			throwException(tokens[0] + " needs one value (size in Bytes)\n");
		// check if valid
		unsigned int max = std::atol(tokens[1].c_str());
		_Vsrvs.back().setMaxBodySize(max);
	}
	else if (dir == ERRPG)
	{
		if (tokens.size() != 3)
			throwException(tokens[0] + " needs two values (error code | path)\n");
		int code = std::atoi(tokens[1].c_str());
		if (code < 400 || code > 511)
			throwException(tokens[1] + " it's not a valid error code (400-511)\n");
		_Vsrvs.back().setErrorPage(code, tokens[2]);
	}
}

int ConfigParser::whichDirective(const std::string &dir) const
{
	if (dir == "host")
		return (HOST);
	else if (dir == "port")
		return (PORT);
	else if (dir == "server_name")
		return (S_NAME);
	else if (dir == "root")
		return (ROOT);
	else if (dir == "index")
		return (INDEX);
	else if (dir == "auto_index")
		return (AUTO);
	else if (dir == "client_max_body_size")
		return (CMBS);
	else if (dir == "error_page")
		return (ERRPG);
	else if (dir == "allow_methods")
		return (ACCMETH);
	else if (dir == "location")
		return (LOCATION);
	else if (dir == "return")
		return (RET);
	else if (dir == "upload_loc")
		return (UPLD);
	else if (dir == "CGI")
		return (CGI);
	else if (dir == "cgi_read_timeout")
		return (CGI_TO);
	return NONE;
}


void ConfigParser::throwException(std::string message) const
{
	throw std::string("ConfigFile: ") + to_string(_lineNum) +" : [" + message;
}

void ConfigParser::removeSpacesFromStart(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), notSpace));
}

void ConfigParser::removeSpacesFromEnd(std::string &s)
{
	s.erase(s.find_last_not_of(" \t") + 1);
}

ConfigParser::ConfigParser(const ConfigParser &) {}

ConfigParser& ConfigParser::operator=(const ConfigParser &) {
	return *this;
}
