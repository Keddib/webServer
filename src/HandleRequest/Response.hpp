#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP


#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>

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
		void setHeader(const std::string&, const std::string&, bool end);
		void addBodyToBuffer(const std::string &);
		void setBodyfile(const std::string &path);
		bool isKeepAlive() const;
		unsigned int getBodySize() const;
		const std::string& getBuffer()const ;
		void display() const;
};

std::string getOsName();
std::string getDate();

#endif
