#ifndef REQINFO_01_HPP
#define REQINFO_01_HPP

#include <string>
#include <vector>

struct ReqInfo
{
	std::string tmpStr;
	std::vector<std::string> tmpHeader;
	const std::string 	&rsource_path;
	const std::string 	&host_name;
	std::string	cachHeader;
	std::string	PATH;
	std::string	cgiPATH;
	std::string CGIfile;
	int					com_srv_index;
	int					meth;
	bool				vers;
	bool				indexon;
	bool				keepAlive;
	bool				isCGI;
	ReqInfo(const std::string &rcp, const std::string &hn, int srvIndex, int mt, bool v, const std::string &cache) :
	rsource_path(rcp), host_name(hn), cachHeader(cache)
	{
		com_srv_index = srvIndex;
		meth = mt;
		vers = v;
		keepAlive = true;
	}
};

#endif
