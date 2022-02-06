#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include "utils.hpp"

class Response
{
	private:
		/* data */
		std::fstream _body;
		std::string _buffer;
		unsigned int _bSize;
		bool _keepAlive;
	public:
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&, bool end);
		void addBodyToBuffer(const std::string &);
		void setBodyfile(const std::string &path);
		void setKeepAlive(bool connection);
		bool isKeepAlive() const;
		unsigned int getBodySize() const;
		const std::string& getBuffer()const ; // i have removed const from return
		std::fstream	&getBody(); // i just added this function
		void display() const;
};

#endif
