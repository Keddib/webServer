#ifndef HEADERS_001_HPP
#define HEADERS_001_HPP

#include "VirtualServer.hpp"
#include "CommonServers.hpp"
#include "ServersInterface.hpp"
#include "Location.hpp"
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <arpa/inet.h>

#define CONFIG_FILE "/Users/keddib/Desktop/Keddib/42Projects/WebServ/config/default.conf"
#define DEFAULT_ROOT "/Users/keddib/Desktop/Keddib/42Projects/WebServ/www/"
#define ERROR_ROOT "/Users/keddib/Desktop/Keddib/42Projects/WebServ/www/error/"
#define D_PORT 8000

/*
** define allowed directives
*/

#define HOST 1
#define PORT 2
#define S_NAME 3
#define CMBS 4
#define ERRPG 5
#define LOCATION 6
#define ROOT 7
#define INDEX 8
#define AUTO 9
#define ACCMETH 10
#define RET 11
#define UPLD 12
#define CGI 13
#define NONE -1

// helper functions

bool notSpace(unsigned char ch);
void ft_split(const std::string &s, std::vector<std::string> &strs);
void simpleTokenizer(std::string &line, std::vector<std::string> &strs);
bool isValidMethod(const std::string &meth);
// add helper func to check valid ip address
// add helper func to check valid port

#endif
