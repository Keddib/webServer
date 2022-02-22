#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "../Includes.hpp"
#include "../MACROS.hpp"
#include "ServersInterface.hpp"

extern	ServersInterface ServI;

/*
** ConfigParser throw exception on error (std::string) contain the error message
*/

class ConfigParser
{
	private:
		std::ifstream _cFile;
		std::vector<VirtualServer> _Vsrvs;
		size_t _lineNum;
		bool _insideServer;
		bool _insideLocation;
	public:
		ConfigParser(std::string file = CONFIG_FILE);
		~ConfigParser();
		void parse();
		void AddServersToServersInterface(); // called after parse;
	private:
		void removeSpacesFromStart(std::string &s);
		void removeSpacesFromEnd(std::string &s);
		ConfigParser(const ConfigParser &);
		ConfigParser& operator=(const ConfigParser &);
		void serverBlock(std::string &line);
		int whichDirective(const std::string &dir) const;
		void throwException(std::string message) const;
		void addDirectiveToServer(int, std::vector<std::string> &);
		void addDirectiveToLocation(int, std::vector<std::string> &);
};


#endif
