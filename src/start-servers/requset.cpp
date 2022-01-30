#include "requset.hpp"

int	GetLocationOf(char *str, const char *target, int size);
int	GetMethod(std::string &str, char *methodHolder, size_t &i);
bool	str_cmp(const char *str, const char *against);
void	InitRecoursePath(std::string &aRsp, std::string &line, size_t &size);
int	GetHttpVersion(std::string &line, size_t index);
int	IndexOf(const char *str, char c);

Request::Request(int srvFd) : srvFd(srvFd)
{
	status= -1;
	bodySize = -1;
}



int	Request::AddToRequest(char *str, int size)
{
	// return 0 means requset not done yet
	// return 1 means return is done
	// return -1 something wron with this requset
	if (status == -1)
	{
		status = ProcessOneLine(str, size);
		if (status != -1)
		{
			str += status;
			size -= status; // what left in string aka str variable
			status = 0;// means ready to start header
			// extract from tmpStr data needed like method...
			if (!InitFromStartLine())
				return -1;
			tmpStr.clear();
		}
	}

	if (!status)
	{
		status = ProcessHeaders(str, size);
		if (status)
			for (auto &str : aHeaders)
				std::cout << str.first << ":::" << str.second << std::endl;
	}
	return 0;
}

// all down has to do with  headers and putting them into the vector
int				Request::ProcessHeaders(char *str, int size)
{
	int tmp;
	while (*str)
	{
		status = ProcessOneLine(str, size);
		if (status == 2 && tmpStr.empty()) // means end of headers
		{
			str += 2; // step over "\r\n" that is at start of str
			return 1; // means done
		}
		if (status != -1)
		{
			// means one header is done so let's creat pair from this tmpStr
			// here i will look at first variable for hostname so i can stor it sepreatl and check body size
			first = tmpStr.c_str();
			tmp = IndexOf(first, ':');
			tmpStr[tmp] = 0;
			// here i need to check if first variable is equal to HostName and with body-size to set some variables ------
			// and i need to test a lot with diffrent headers -------
			second = first + tmp + 1; // what is after ":" character
			aHeaders.push_back(std::make_pair(first, second));
			tmpStr.clear();
			str += status;
		}
		else
			break ;
	}
	return 0; // means headers not done yet
}

// all down has to do with start line nothing more
void	InitRecoursePath(std::string &aRsp, std::string &line, size_t &size)
{
	size_t s = line.size();
	size_t index;
	aRsp.reserve(s - size);
	for (index = ++size; index < s; ++index)
		if (!(line[index] == ' ' || line[index] == '\t'))
			break ;
	// now index has start of resource
	for (;index < s; ++index)
	{
		if (line[index] == ' ' || line[index] == '\t')
			break ;
		aRsp.push_back(line[index]);
	}
	size = index;
}


int	GetHttpVersion(std::string &line, size_t index)
{
	size_t s = line.size();
	const char *tmp;
	for (; index < s; ++index)
		if (!(line[index] == ' ' || line[index] == '\t'))
			break ;
	// now index has start of HTTP version
	
	tmp = line.c_str() + index;
	if (str_cmp(tmp, "HTTP/1.0"))
		return 0;
	else if (str_cmp(tmp, "HTTP/1.1"))
		return 1;
	return -1;
	
}

bool	Request::InitFromStartLine()
{
	// this will make method // to  bet GET OR POST OR DELETE give each a number above -1
	// and resourcPath string
	// if version is 1.0 version variable will be false if 1.1 will be true
	// clear string after
	// if start line valid return true otherwise return false
	size_t	start;
	method = GetMethod(tmpStr, methodHolder, start);
	if (method == -1)
		return false; // means method is uknown
	// i need now to get resource path
	InitRecoursePath(aResourcPath, tmpStr, start);
	start = GetHttpVersion(tmpStr, start);
	if (start == -1)
		return false;
	version = start;
	return true;
}

int				Request::ProcessOneLine(char *str, int size)
{
	int tmp;
	tmp = tmpStr.size();
	if (tmp && tmpStr[tmp - 1] == '\r' && *str == '\n')
	{
		// start line is done
		tmpStr += *str;
		return 1; // number will be used in AddToRequest to parse rest of str probably headers
	}

	tmp = GetLocationOf(str, "\r\n", size);
	if (tmp != -1)
	{
		str[tmp] = '\0'; // replacing "\r" by "\0" so string can add only what before "\r"
		tmpStr += str;
		tmp += 2; // go over "\n" for later use probably headers
		return tmp;
	}
	return -1;
}

int	GetLocationOf(char *str, const char *target, int size)
{
	for (int i = 0; i < size - 1; ++i)
	{
		if (str[i] == target[0])
		{
			if (str[i + 1] == target[1])
				return i;
		}
	}
	return -1; // no "\r\n" found here
}

bool	str_cmp(const char *str, const char *against)
{
	int i = 0;
	while (str[i] && str[i] == against[i])
		++i;
	return (str[i] == against[i]);
}

int	IndexOf(const char *str, char c)
{
	int i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return i;
		++i;
	}
	return -100; // not found i'm returning bif negative number so if it happend it will give me segv signla and i can spot that in debugging
}


int	GetMethod(std::string &str, char *methodHolder, size_t &i)
{
	// if methode not known will return -1
	for (i = 0; i < LONGEST_METHOD; ++i){
		if (str[i] == ' ' || str[i] == '\t')
			break ;
		methodHolder[i] = str[i];
	}
	methodHolder[i] = 0;
	if (str_cmp(methodHolder, GET_STR))
		return GET;
	else if (str_cmp(methodHolder, POST_STR))
		return POST;
	else if (str_cmp(methodHolder, DELETE_STR))
		return DELETE;
	return UKNOWNMETHOD;
}
