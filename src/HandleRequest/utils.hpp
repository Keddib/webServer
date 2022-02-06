#ifndef UTILS_07_HPP
#define UTILS_07_HPP

#include "../MACROS.hpp"

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

typedef struct	FileInfo_s
{
	int64_t		size;
	std::string	Mtime;
	std::string Ftype;
}				FileInfo;

bool isFileAccessible(const std::string &path);
int getFileInfo(const std::string &fileName, FileInfo &fileI);
std::string getFileType(const std::string &path);
const char *getErrorPage(int error);
const char *getErrorMessage(int error);
std::string getOsName();
std::string getDate();
std::string ListDirectory(const std::string &Dir, const std::string &name);
std::string lookForIndexInDirectory(const std::string &Dir, const std::vector<std::string> &indexes, int &error);

#endif
