#include <iostream>
#include <memory>
#include <map>
#include "alloc.hpp"

struct MyObj
{
	int l[100];
};

int main()
{
	std::allocator<std::pair<int, MyObj>> alloc;
	std::map<int, MyObj, std::less<int>, MyAlloc<std::pair<const int, MyObj>>> mp;
	while (1)
	{
		mp.insert(std::make_pair(1, MyObj()));
		mp.erase(1);
	}
}
