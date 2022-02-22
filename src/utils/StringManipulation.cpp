#include "../Includes.hpp"
#include "../MACROS.hpp"

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

char asciitolower(char in)
{
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

char asciiToUpper(char in)
{
	if (in <= 'z' && in >= 'a')
		return in - ('z' - 'Z');
	else if (in == '-')
		return ('_');
	return in;
}

inline short count_num_of_digit(long long n)
{

	short i = (n < 0); // start with 1 if number is negative
	for (; n != 0; ++i)
		n = n / 10;
	return i;
}

std::string	to_string(long long n)
{
	if (n == 0)
		return std::string("0");
	short nd = count_num_of_digit(n);
	std::string str(nd, '-');
	n = (n < 0) ? (n * (-1)) : n;
	for (short i = nd - 1; n != 0; --i)
	{
		str[i] = (n % 10) + 48;
		n = n / 10;
	}
	return str;
}

std::string ConvertIPtoString(uint32_t IP)
{
	char str[INET_ADDRSTRLEN];
	struct sockaddr_in sa;
	sa.sin_addr.s_addr = IP;
	inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);
	return str;
}

bool	my_strncmp(const char *s1, const char *s2, size_t n)
{
	for (size_t i = 0; i < n; ++i)
		if (tolower(s1[i]) != tolower(s2[i]))
			return false;
	return true;
}
