#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{

	std::fstream l;
	l.open(argv[1], std::fstream::out);
	l << "hello";

	l.close();
	std::fstream file;

	file.open(argv[1]);
	std::cout << file.is_open() << std::endl;

	char data[100];
	
	file.read(data, 5);

	std::cout << data << std::endl;
}
