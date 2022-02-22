#ifndef RESPONSE_001_HPP
#define RESPONSE_001_HPP

#include "../Includes.hpp"
#include "../configuration/ServersInterface.hpp"

extern ServersInterface ServI;

class Response
{
	private:
		/* data */
		int	cmSrvFd;
		std::fstream _body;
		std::string _bodyFileName;
		std::string _host;
		int64_t _bSize;
		int64_t _headersSize;
		int _PID;
		time_t _startTime;
		time_t _cgiTO;
		bool _keepAlive;
		bool _isFileUsed;
		bool _isReady;
	public:
		std::string _buffer;
		Response();
		~Response();
		void setStartLine(const std::string&, int, const std::string &);
		void setHeader(const std::string&, const std::string&, bool end = 0);
		void setHeader(const std::vector<std::string> &headers);
		void setHeader(std::string header);
		void addBodyToBuffer(const std::string &);
		void setBodyfile(const std::string &path);
		void setBodyfile( unsigned int size);
		void setKeepAlive(bool connection);
		void setBodySize(int64_t bsize);
		void setNotReady(time_t time, int PID, const std::string &file,  bool ready=false);
		void setTimeout(time_t time);
		void setHost(const std::string &);
		void setHeaderSize(size_t size);
		bool isKeepAlive() const;
		unsigned int getBodySize() const;
		const std::string& getBuffer() const; // i have removed const from return
		std::fstream	&getBody(); // i just added this function
		const std::string &getBodyFile() const;
		bool isFileUsed() const;
		int	getCommonServerIndex() const;
		void	setCommonServerIndex(int s);
		bool	isReady();
		void display();
	private:
		bool	getResponse(int status);
		bool	getTimeoutResponse();
		void	DocumentResponse(int code);
		void	ClientRedirectResponse(CGIIresInfo &resInfo);
		void	getErrorResponse(int status);
		bool	setUserErrorPage(int error);
};

CGIIresInfo ParseCGIresponse(Response *res, const std::string &_CGIfile);

#endif
