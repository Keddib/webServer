#include <fstream>
#include <map>
#include <stdlib.h>
#include <iostream>

int main()
{
/*	std::fstream file;

	file.open("file");

	char stroge[1024];
	stroge[10] = 0;
	file.read(stroge, 10);

	std::cout << stroge << std::endl;
	file.seekg(3);
	file.read(stroge, 10);
	std::cout << stroge << std::endl;*/

	std::map<int, int> mp;

	std::map<int, int>::iterator i(mp.find(2));

	std::cout << (i != mp.end()) << std::endl;
	

}
