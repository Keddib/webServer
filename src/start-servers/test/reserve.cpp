#include <iostream>
#include <vector>

struct MyObj
{
	MyObj()
	{
		str = "valid string";
	}
	std::string str;
};

int main()
{
	std::vector<MyObj> v;
	int size;

	std::cout << "enter a size: " ;
	std::cin >> size;
	for (int i = 0; i < size; ++i)
		v.push_back(MyObj());

	v.reserve(size * 2);
	for (auto &val: v)
		std::cout << val.str << std::endl;


	std::cout << v.capacity() << std::endl;
}


