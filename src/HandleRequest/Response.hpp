#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include <string>
#include <unistd.h>
#include <fstream>

class Response
{
	private:
		/* data */
		std::fstream _body;
		std::string _buffer;
		unsigned int _bSize;
		bool _keepAlive;
		bool _bodyExcite;
	public:
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&);
		bool isKeepAlive();
		unsigned int getBodySize();
		const std::string& getBuffer();
		void setBodyfile(const std::string &path);
};

#endif
