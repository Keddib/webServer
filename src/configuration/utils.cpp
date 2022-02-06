#include "confHeaders.hpp"

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

int isValidMethod(const std::string &meth)
{
	if (meth == "GET")
		return GET;
	else if (meth == "POST")
		return POST;
	else if (meth == "DELETE")
		return DELETE;
	return 0;
}

int findLocationURL(const std::string &path, const std::string &prefix)
{
	size_t psize = prefix.size();
	if (path.size() < psize)
		return -1;
	size_t i = 0;
	for (;i < psize; i++)
	{
		if (path[i] != prefix[i])
			return -1;
	}
	return i;
}
