#include "headers.hpp"

bool notSpace(unsigned char ch)
{
	return !std::isspace(ch);
}

void simpleTokenizer(std::string &line, std::vector<std::string> &strs)
{
	std::stringstream ss(line);
	std::string token;
	while (ss >> token) {
		strs.push_back(token);
	}
}

void ft_split(const std::string &s, std::vector<std::string> &strs)
{
	typedef std::string::const_iterator iter;

	iter i = s.begin();
	while (i != s.end())
	{
		i = find_if(i, s.end(), notSpace);

		iter j = find_if(i, s.end(), isspace);

		if (i != s.end())
			strs.push_back(std::string(i, j));
		i = j;
	}
}

bool isValidMethod(const std::string &meth)
{
	return (meth == "GET" || meth == "POST" || meth == "DELETE");
}
