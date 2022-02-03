#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include <string>
#include <unistd.h>

class Response
{
	private:
		/* data */
		std::string _header;
		/// fstream 
		int _bodyFD;
		unsigned int _bSize;
		bool _keepAlive;
		// is there a body or not
	public:
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&);
		void setBodyFD(int);
		bool isKeepAlive();
		int getBodyFD();
		unsigned int getBodySize();
		const std::string& getHeader();
};

#endif
