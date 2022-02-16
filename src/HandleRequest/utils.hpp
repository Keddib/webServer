#ifndef UTILS_07_HPP
#define UTILS_07_HPP

#include "../MACROS.hpp"
#include "ReqInfo.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include <arpa/inet.h>
#include <netdb.h>
#include <algorithm>
#include <cstdlib>

extern bool DONE;

typedef struct	FileInfo_s
{
	int64_t		size;
	std::string	Mtime;
	std::string Ftype;
	bool		keepAlive;
}				FileInfo;

std::string ConvertIPtoString(uint32_t IP);
bool isFileAccessible(const std::string &path);
int getFileInfo(const std::string &fileName, FileInfo &fileI);
std::string getFileType(const std::string &path);
void getErrorPage(int error, std::string &page);
const char *getErrorMessage(int error);
std::string getOsName();
std::string getDate();
std::string ListDirectory(const std::string &Dir, const std::string &name);
std::string lookForIndexInDirectory(const std::string &Dir, const std::vector<std::string> &indexes, int &error);
std::string getMyIP();
char asciitolower(char in);
char asciiToUpper(char in);
std::string	to_string(long long n);
bool fileHasextension(const std::string &file, const std::string &exten);


#endif
