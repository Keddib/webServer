#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include "utils.hpp"

class Response
{
	private:
		/* data */
		std::fstream _body;
		std::string _buffer;
		int64_t _bSize;
		bool _keepAlive;
	public:
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&, bool end);
		void setHeader(const std::vector<std::string> &headers);
		void addBodyToBuffer(const std::string &);
		void setBodyfile(const std::string &path);
		void setKeepAlive(bool connection);
		void setBodySize(int64_t bsize);
		bool isKeepAlive() const;
		unsigned int getBodySize() const;
		const std::string& getBuffer()const ; // i have removed const from return
		std::fstream	&getBody(); // i just added this function
		void display();
};

#endif
