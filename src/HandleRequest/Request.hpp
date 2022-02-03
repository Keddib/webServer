#ifndef REQUEST_001_HPP
#define REQUEST_001_HPP

#include <string>
#include <unistd.h>
#define GET 20
#define POST 21
#define DELETE 23

class Request
{
	private:
		/* data */
		std::string _hostName;
		std::string _resource;
		size_t _commonServ;
		int _method;
		bool _version;
		// vector of headers
		// int _bodyFD=-1;
	public:
		Request(int, int, const std::string &, bool);
		~Request();
		int getMethod() const;
		const std::string& getResource() const;
		const std::string& getHostName() const;
		bool whichVersion() const;
		size_t getCSindex() const;
};

#endif
