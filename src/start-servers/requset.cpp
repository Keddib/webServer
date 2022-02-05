#include "requset.hpp"

void	STOP_HERE();


int	GetLocationOf(char *str, const char *target, long size);
int	GetMethod(std::string &str, char *methodHolder, size_t &i);
bool	str_cmp(const char *str, const char *against);
void	InitRecoursePath(std::string &aRsp, std::string &line, size_t &size);
int	GetHttpVersion(std::string &line, size_t index);
int	IndexOf(const char *str, char c);
long	GetBodySize(const char *str);
void	fillFileName(int num, char* str);

Request::Request(int confd, int comServerIndex)
{
	INIT_AT_CONSTRUCTION(confd, comServerIndex);
}

void				Request::INIT_AT_CONSTRUCTION(int confd, int comServerIndex)
{
	this->comServerIndex = comServerIndex;
	connFD = confd;
	status = -1;
	bodySize = -1;
	isChuncked = -1;
	hasBeenRead = 0;
	strcpy(fileName, "._req_body");
	chunkedBodyState = true;
	booltmp = false;
}

Request::Request(const Request &cp)
{
	*this = cp;
}

Request&	Request::operator=(const Request &rhs)
{
	this->comServerIndex = rhs.comServerIndex;
	connFD = rhs.connFD;
	status = rhs.status;
	bodySize = rhs.bodySize;
	isChuncked = rhs.isChuncked;
	hasBeenRead = rhs.hasBeenRead;
	strcpy(fileName, "._req_body"); // stay like this not related here
	chunkedBodyState = rhs.chunkedBodyState;
	booltmp = rhs.booltmp;
	return *this;
}

Response			*Request::StartLineParsing(char **str, int &size)
{
	// processing start line
	status = ProcessOneLine(*str, size);
	if (status != -1)
	{
		(*str) += status;
		size -= status; // what left in string aka str variable
		status = 0;// means ready to start header
		// extract from tmpStr data needed like method...
		/*if (!InitFromStartLine())
			return NULL;
			//return -1; // i will change this bec in case if error i should return responce not immdiate answer*/
		Restmp = InitFromStartLine();
		if (Restmp)
			return Restmp;
		tmpStr.clear();
	}
	return NULL;
}

Response	*Request::AddToRequest(char *str, int size)
{
	endStr = str + size;
	Restmp = NULL;
	if (status == -1)
	{
		Restmp = StartLineParsing(&str, size);
		Restmp->display();
		if (Restmp)
			return Restmp;
		exit(1); /////// remove this line
	}
	if (!status)
	{
		// processing headers
		status = ProcessHeaders(&str, size);
		if (status == -1)
			return NULL;
			//return -1; // problem accured handel later
	}

	if (status == 1)
	{
		// processing body
		if (!booltmp)
		{
			booltmp = true;
			// here make sure that this branch will only be excuted once
			if (isChuncked == 1)
			{
				fillFileName(connFD, fileName);
				bodyFileObj.open(fileName, std::fstream::out); // creating file
			}
			else if (bodySize != -1)
			{
				if (bodySize < MAX_BODY_SWITCH)
					bodyString.reserve(bodySize);
				else {
					fillFileName(connFD, fileName);
					bodyFileObj.open(fileName, std::fstream::out); // creating file
				}
			}
			else // means no body
				return NULL;
				//return 1; // wich indacte done reading request
		}
		if (ProcessBody(str, endStr - str)) {
			bodyFileObj.close();
			STOP_HERE();
			return NULL;
		//	return true;
		}
		// working here check if you read the body correctly or not check chunked and with Content-Length
	}
	return NULL;
}

// body processing

bool				Request::ProcessBody(char *str, long size)
{
	// this function will return true if body is done if not it will return false
	int tmp;

	tmp = HowToReadBody();
	if (tmp == 1)
	{
		if (ReadBody(str, size))
			return true;
	}
	else if (tmp == -1)
	{
		if (ReadByChunck(str, size))
			return true;
	}
	else
		return true;
	return false;
}

bool	Request::ReadBody(char *str, long size)
{
	// be carefull in case of file writing
	// this is easy just read entire body and store it in file
	// if Content-Length is under MAX_BODY_SWITCH body will be string
	// if bigger will b stord and file
	if (bodySize < MAX_BODY_SWITCH)
		return BodyStringCase(str, size);
	// from here i will handle body as file
	// will create file with comServerIndex as name
	return BodyFileCase(str, size);
}

bool	Request::BodyStringCase(char *str, long size)
{
	// means that body small enough to be stored in string obj
	if (hasBeenRead + size < bodySize)
	{
		bodyString += str;
		hasBeenRead += size;
		return false;
	}
	str[bodySize - hasBeenRead + 1]  = 0; /* in this case i'm gonna take just what is needed for example if str "bbaaaa" and only two are still needed so i will add only two and done*/
	bodyString += str;
	return true;
}

bool	Request::BodyFileCase(char *str, long size)
{
	if (hasBeenRead + size < bodySize)
	{
		bodyFileObj << str;
		hasBeenRead += size;
		return false;
	}
	str[bodySize - hasBeenRead] = 0;
	bodyFileObj << str;
	return true;
}

bool	Request::ReadByChunck(char *str, long size)
{
	// working here ----------------------------------------------------------------------------
	// chunked body will be stored in file
	int tmp;
	while (*str)
	{
		if (chunkedBodyState)
		{
			// if chunkedBodyState true means that
			// i will be looking for how much client will supply of data
			tmp = ProcessOneLine(str, size);
			if (tmp != -1)
			{
				// this almost always will be true
				// and it means that number that need to be read is stored in tmpStr variable
				bodySize = strtol(tmpStr.c_str(), NULL, 16);
				if (bodySize == 0)
					return true; // which means that the body is done dealing with
				tmpStr.clear();
				str += tmp; // MOVING str after "\r\n"
				size -= tmp; // this calc here is wrong-------------
				chunkedBodyState = false;
				hasBeenRead = 0;
			}
		}
		if (!chunkedBodyState)
		{
			if (BodyFileCase(str, size)) // and this call here is getting wrong str
			{
				//bodyFileObj<< "\n"; // i do not know now should add new line after or not
				chunkedBodyState = true; // swith again to wait for other data
				str += (bodySize - hasBeenRead + 2); // this important
				// example
				// bodySize = 7 and hasBeenRead = 0
				// BODY WAS "*******78544\r\n********"
				// BodyFileCase() will return true because read 7 but it still data after i should not just waste that
				// so by (bodySize - hasBeenRead + 2) i'm moving str after first 7 so str will be (str = "78544\r\n********")
			}
			else
				break ;
		}
	}
	return false;
}

int					Request::HowToReadBody() const
{
	// return 1; if not chunked and should body exist
	// return 0; if there's no body
	// return -1; if body is chunked
	if (isChuncked == 1)
		return -1;
	if (bodySize != -1)
		return 1;
	return 0;
}




// all down has to do with  headers and putting them into the vector
int				Request::ProcessHeaders(char **str, int size)
{
	// return 1 means that headers are done
	// return 0 means that headers not done yet
	// return -1 if problem ("Key ' ': value") that ' ' is the problem here it should not
	bool	opfold;
	while (*str)
	{
		status = ProcessOneLine(*str, size);
		if (IsHeadersDone(str))
			return 1;
		if (status != -1)
		{
			if (!FirstSecondFromHeaderLine(opfold))
				return -1;
			if (!opfold)
				TakeInfoFromHeaders(str);
			else
			{
				// by the way this entire branch has not been tested yet
				std::string &opStr = aHeaders.back().second;
				opStr += ", ";
				opStr += first;
				(*str) += tmpStr.size();
				tmpStr.clear();
			}
		}
		else
			break ;
	}
	return 0; // means headers not done yet
}


bool				Request::IsHeadersDone(char **str)
{
	// this function will return true if we reach the end of headers
	// which means that we got "\r\n"
	// status = -1 means that there's no "\r\n"
	// status <= 2 means that eather we found "\r\n" or "\n" at start
	// tmpStr.size() <= 1 means if tmpStr=0 means empty line if 1 means that ther's tmpStr[0] = '\r'
	/* EXMPLES */
	// 1: tmpStr = '' and (status = 2 => str = "\r\n****")
	// 2: tmpStr = '\r' and str = '\n**'
	// these are the cases that this branch will be true
	if (status != -1 && status <= 2 && tmpStr.size() <= 1)
	{
		(*str) += 2;
		return true;
	}
	return false;
}

void				Request::TakeInfoFromHeaders(char **str)
{
	// this function will collect some informmation from header
	// for in Request needed informmation like Content-Length
	// or for respone info like Host
	if (str_cmp(first, "Host"))
		aHostName = second;
	else
		aHeaders.push_back(std::make_pair(first, second));
	if (isChuncked == -1 && str_cmp(first, "Transfer-Encoding"))
	{
		if (str_cmp(second, "chunked"))
			isChuncked = 1;
		else
			isChuncked = 0; // here mybe error bec Transfer-Encoding: gzip for example which we do not support
	}
	else if (isChuncked != 1 && str_cmp(first, "Content-Length"))
		bodySize = GetBodySize(second);
	tmpStr.clear();
	(*str) += status;
}


bool	Request::FirstSecondFromHeaderLine(bool &opfold)
{
	// this function will change one tmpStr line
	// to first second pointers sperated by ":"
	/* EXAMPLE */
	// Host: localhost
	// first = Host
	// second = localhost
	int tmp;
	opfold = false;
	first = tmpStr.c_str();
	tmp = IndexOf(first, ':');
	if (tmp == -1)
	{
		opfold = true;
		return true;
	}
	tmpStr[tmp] = 0;
	if (tmpStr[tmp - 1] == ' ')
		return false;
	second = first + tmp + 2; // what is after ":" character because i'm assuming that ther's always one space at first that's why i go over that one space maybe later optimiziation
	return true;
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


Response	*Request::InitFromStartLine()
{
	// this will make method // to  bet GET OR POST OR DELETE give each a number above -1
	// and resourcPath string
	// if version is 1.0 version variable will be false if 1.1 will be true
	// clear string after
	// if start line valid return true otherwise return false
	size_t	start;
	method = GetMethod(tmpStr, methodHolder, start);
	if (method == NOT_IMPLEMENTED)
		return errorRespo.getResponse(comServerIndex, NOT_IMPLEMENTED_STATUS_CODE);
	if (method == UKNOWNMETHOD)
		return errorRespo.getResponse(comServerIndex, SYNTAX_STATUS_CODE);
	// i need now to get resource path
	InitRecoursePath(aResourcPath, tmpStr, start);
	start = GetHttpVersion(tmpStr, start);
	if (start == HTTP_VERSION_NOT_SUPPORTED)
		return errorRespo.getResponse(comServerIndex, HTTP_VERSION_NOT_SUPPORTED_STATUS_CODE);
	if (start == SYNTAX_ERROR)
		return errorRespo.getResponse(comServerIndex, SYNTAX_STATUS_CODE);
	version = start;
	return NULL;
}

int				Request::ProcessOneLine(char *str, long size)
{
	int tmp;
	tmp = tmpStr.size();
	if (tmp && tmpStr[tmp - 1] == '\r' && *str == '\n')
	{
		// start line is done
		tmpStr.erase(tmp - 1, 1);
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
	else
		tmpStr += str;
	return -1;
}

// c functions are down nothing els

long	GetBodySize(const char *str)
{
	while (*str && (*str == ' ' || *str == '\t'))
		++str;
	return atoi(str);
}


int	GetHttpVersion(std::string &line, size_t index)
{
	size_t s = line.size();
	char c;
	char *tmp;
	for (; index < s; ++index)
		if (!(line[index] == ' ' || line[index] == '\t'))
			break ;
	// now index has start of HTTP version

	tmp = const_cast<char *>(line.c_str() + index);
	c = tmp[5]; // if tmp = HTTP/F    c will be F
	tmp [5] = 0; // tmp will be here tmp = HTTP/
	if (str_cmp(tmp, "HTTP/"))
	{
		tmp[5] = c;
		tmp += 5;
		if (str_cmp(tmp, "1.1"))
			return HTTP_VERSION_SUPPORTED + 1;
		else if (str_cmp(tmp, "1.0"))
			return HTTP_VERSION_SUPPORTED;
		return HTTP_VERSION_NOT_SUPPORTED;
	}
	return SYNTAX_ERROR;
}

int	GetLocationOf(char *str, const char *target, long size)
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
	return -1; // not found i'm returning bif negative number so if it happend it will give me segv signla and i can spot that in debugging
}


int	GetMethod(std::string &str, char *methodHolder, size_t &i)
{
	// if methode not known will return -1
	for (i = 0; i < LONGEST_METHOD; ++i)
	{
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
	else if (!strcmp(methodHolder, OPTIONS_STR) || !strcmp(methodHolder, HEAD_STR) || !strcmp(methodHolder, PUT_STR) || !strcmp(methodHolder, PATCH_STR))
		return NOT_IMPLEMENTED;
	return UKNOWNMETHOD;
}


void	fillFileName(int num, char* str)
{
	// will be used to genrate name for the file that i'm creating
	// given 32 returns ./req-files/23 as char*
    int i = 10; // this number is dependent on what is already in str variable

    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }
    str[i] = '\0';
}


void	AVAILABLE_COMMANDS()
{
	std::cout << "exit: to exit from process\n";
	std::cout << "cont: to continue excution\n";
	std::cout << "id: print pid of this proccess\n";
}


void	STOP_HERE()
{
	std::string str;

	do {
		str.clear();
		std::cout << ">> ";
		std::cin >> str;
		if (str == "help")
			AVAILABLE_COMMANDS();

		if (str == "exit")
			exit(10);
		if (str == "id")
			std::cout << getpid() << std::endl;
	}while (str != "cont");
}
