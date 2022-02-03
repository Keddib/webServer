#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>



int main()
{
	std::string filename = "/path/to/file";
	struct stat result;
	if(stat(filename.c_str(), &result)==0)
	{
		auto mod_time = result.st_mtime;
		std::cout << mod_time << "\n";
	}
}


GET / HTTP/1.1
Host: localhost
Accept-Encoding: gzip,
deflat, br
Accept-Language: en-US,en;q=0.5
