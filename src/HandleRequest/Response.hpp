#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include "utils.hpp"

class Response
{
	private:
		/* data */
		int	cmSrvFd;
		std::fstream _body;
		std::string _buffer;
		int64_t _bSize;
		bool _keepAlive;
		bool _isFileUsed;
	public:
		std::string deleteFile;
	public:
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&, bool end = 0);
		void setHeader(const std::vector<std::string> &headers);
		void addBodyToBuffer(const std::string &);
		void setBodyfile(const std::string &path);
		void setBodyfile(const std::string &path, unsigned int size);
		void setKeepAlive(bool connection);
		void setBodySize(int64_t bsize);
		bool isKeepAlive() const;
		unsigned int getBodySize() const;
		const std::string& getBuffer()const ; // i have removed const from return
		std::fstream	&getBody(); // i just added this function
		bool isFileUsed() const;
		int	getCommonServerIndex() const;
		void	setCommonServerIndex(int s);
		void display();
};

#endif
