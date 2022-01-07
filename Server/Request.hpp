#ifndef REQUEST_77_HPP
#define REQUEST_77_HPP

#include <string>
#include <iostream>


class Request
{
	private:
		/* data */
		uint8_t _method;
		std::string _path;
		uint8_t _version;
		uint32_t _contentLenght;
		bool _isCompleted;

	public:
		Request(/* args */);
		~Request();
		void readStartLine();
		void readheaders();
};

#endif
