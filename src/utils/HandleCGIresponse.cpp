#include "../MACROS.hpp"
#include "../Includes.hpp"
#include "../Response/Response.hpp"

CGIIresInfo ParseCGIresponse(Response *res, const std::string &_CGIfile)
{
	int delm;
	char _buff[BUFFER_SIZE];
	CGIIresInfo _cgii_res_info;
	size_t bytesRead = 0;
	std::fstream _CGIres(_CGIfile, std::fstream::in | std::fstream::binary);
	if (_CGIres.is_open())
	{
		do {
			_CGIres.getline(_buff, BUFFER_SIZE);
			bytesRead += _CGIres.gcount();
			if (_CGIres.gcount() > BUFFER_SIZE)
			{
				_cgii_res_info.error = 502;
				return _cgii_res_info;
			}
			_buff[_CGIres.gcount() - 1] = '\n';
			_buff[_CGIres.gcount()] = 0x00;
			if (_buff[0] == '\r' || _buff[0] == '\0')
				break ; // means that empty lines after header fields
			delm = IndexOf(_buff, ':');
			if (delm == -1)
				continue;
			if (!_cgii_res_info.cont_type && my_strncmp(_buff, "Content-Type", delm))
				_cgii_res_info.cont_type = true;
			else if (!_cgii_res_info.location && my_strncmp(_buff, "Location", delm))
				_cgii_res_info.location = true;
			else if (!_cgii_res_info.status.first && my_strncmp(_buff, "Status", delm)) // myabe the status shuold be lower case
			{
				_cgii_res_info.status.first = true; // Status: 302 Found
				_cgii_res_info.status.second = std::atoi(_buff + delm + 1); // if i need to take at as number write atoi(str)
				continue ;
			}
			res->setHeader(_buff);
		}
		while (_buff[0]); // it will break bec of empty line after headers
		// after this loop file pointer will be at body if there's one
		_CGIres.read(_buff, 10);
		_cgii_res_info.bodyExist = _CGIres.gcount();
		_CGIres.close();
		res->setHeaderSize(bytesRead);
		return _cgii_res_info;
	}
	std::cout << _CGIfile <<" file not opened \n";
	_cgii_res_info.error = 500;
	return _cgii_res_info;
}
