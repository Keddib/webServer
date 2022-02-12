#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdio.h>

int main()
{
	std::cout << sizeof(long long) << std::endl;
	std::fstream file;
	std::vector<long long> inserted, deleted;

	std::size_t tmp;
	file.open("./log");

	char line[30];
	int i;
	do
	{
		i = 0;
		file.getline(line, 30);
		while (line[i] != '|')
			++i;
		line[i] = 0;
		if (line[0] == 'i')
			inserted.push_back(std::stoll(line + 8, &tmp, 16));
		else
			deleted.push_back(std::stoll(line + 8, &tmp, 16));
	}while (line[0]);


	std::vector<long long>::iterator id;
	std::vector<long long>::iterator ii;


	id = deleted.end() - 1;
	bool found;
	int counter = 0;
	for (; id >= deleted.begin(); --id)
	{
		found = false;
		ii = inserted.end() - 1;
		for (; ii >= inserted.begin(); --ii)
		{
			if (*id == *ii)
			{
				found = true;
				inserted.erase(ii);
				break ;
			}
		}
		if (!found)
		{
			printf("was not allocated 0x%llx at line from buttom%d\n", *id, counter);
			file.close();
			exit(1);
		}
		deleted.erase(id);
		id = deleted.end() - 1;
		++counter;
	}
	std::cout << "all good\n";
	file.close();

}
