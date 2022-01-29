#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include <string>

class Response
{
	private:
		/* data */
		std::string _startLine;
		std::string _headers;
		int _bodyFD;
	public:
		Response(/* args */);
		~Response();
		setStartLine(const std::string&, int, const std::string &);
};

#endif
