#include "requset.hpp"


void	STOP_HERE();


const char	*remove_speces_at_end_start(char *str, bool);
int	GetLocationOf(char *str, const char *target, long size);
int	GetMethod(std::string &str, char *methodHolder, size_t &i);
bool	str_cmp(const char *str, const char *against);
void	InitRecoursePath(std::string &aRsp, std::string &line, size_t &size);
int	GetHttpVersion(std::string &line, size_t index);
int	IndexOf(const char *str, char c);
long	GetBodySize(const char *str);
void	fillFileName(int num, char* str);

Request::Request(int confd, int comServerIndex, const struct sockaddr_in &cl_inf)
{
	client_info.first = inet_ntoa(cl_inf.sin_addr);
	client_info.second = cl_inf.sin_port;
	INIT_AT_CONSTRUCTION(confd, comServerIndex);
}

void				Request::INIT_AT_CONSTRUCTION(int confd, int comServerIndex)
{
	portStr = "80";
	totalRead = 0; // added
	is_req_alive = true;
	startTime = std::time(NULL);
	this->comServerIndex = comServerIndex;
	connFD = confd;
	status = -1;
	bodySize = -1;
	isChuncked = -1;
	hasBeenRead = 0;
	strcpy(fileName, "/tmp/.");
	chunkedBodyState = true;
	method = -1;
	version = 0;
	booltmp = false;
	bzero(methodHolder, LONGEST_METHOD + 1);
	bodyFileObj.close();
	bodyString.clear();
	tmpStr.clear();
	aResourcPath.clear();
	aHeaders.clear();
	aHostName.clear();
	Restmp = NULL;
	first = second = endStr = NULL;
	Restmp = NULL; //22
}

const char	*Request::getFileName() const
{
	return fileName;
}

void	Request::DisableRequest()
{
	is_req_alive = false;
}

bool				Request::isStillValid(std::time_t cur_t)
{
	if (is_req_alive)
		return (cur_t - startTime < TIMEOUT);
	return true;
}

void	Request::RESET()
{
	// i will pass the old comServerIndex and connFD
	INIT_AT_CONSTRUCTION(connFD, comServerIndex);
}

void	Request::DISPLAY()
{
	std::cout << "aHeaders size: " << aHeaders.size() << "\n";
	std::cout << "body str: " << bodyString.size() << "\n";
	std::cout << "tmpStr " << tmpStr.size() << "\n";
	std::cout << "chunked body state: " << chunkedBodyState << "\n";
	std::cout << "bodyFileObj: " << bodyFileObj.is_open() << "\n";
	std::cout << "hasBeenRead: " << hasBeenRead << "\n";
  	std::cout << "isChuncked: " << isChuncked << "\n";
	std::cout << "status: " << status << "\n";
	std::cout << "comServerIndex: " << comServerIndex << "\n";
	std::cout << "method: " << method << "\n";
	std::cout << "aResourcPath: " << aResourcPath << "\n";
	std::cout << "version: " << version << "\n";
	std::cout << "aHostName: " << aHostName << "\n";
	std::cout << "methodHolder: " << methodHolder << "\n";
	std::cout << "fileName: " << fileName << "\n";
	std::cout << "booltmp: " << booltmp << "\n";
}

Request::Request(const Request &cp)
{
	*this = cp;
}

Request&	Request::operator=(const Request &rhs)
{
	portStr = rhs.portStr;
	client_info = rhs.client_info;
	totalRead = rhs.totalRead; // added
	is_req_alive = rhs.is_req_alive;
	startTime = rhs.startTime;
	comServerIndex = rhs.comServerIndex;
	connFD = rhs.connFD;
	status = rhs.status;
	bodySize = rhs.bodySize;
	isChuncked = rhs.isChuncked;
	hasBeenRead = rhs.hasBeenRead;
	strcpy(fileName, rhs.fileName);
	chunkedBodyState = rhs.chunkedBodyState;
	method = rhs.method;
	version = rhs.version;
	booltmp = rhs.booltmp;
	bzero(methodHolder, LONGEST_METHOD + 1);
	bodyString = rhs.bodyString;
	tmpStr = rhs.tmpStr;
	aResourcPath = rhs.aResourcPath;
	aHeaders = rhs.aHeaders;
	aHostName = rhs.aHostName;
	Restmp = rhs.Restmp;
	first = second = endStr = NULL;
	Restmp = NULL;
	return *this;
}


Response			*Request::StartLineParsing(char **str, int &size)
{
	// processing start line
	status = ProcessOneLine(*str, size);
	if (status != -1)
	{
		(*str) += status;
		size -= status;
		status = 0;
		Restmp = InitFromStartLine();
		if (Restmp)
			return Restmp;
		tmpStr.clear();
	}
	return NULL;
}

void	Request::ResourseDecoding()
{
	 size_t size = aResourcPath.size();
	 const char *str;
	 char c;
	 tmpRSP.clear();
	 for (size_t i = 0; i < size; ++i)
	 {
	 	if (aResourcPath[i] == '?')
	 	{
	 		 std::string strTmp = aResourcPath.c_str() + i; // copy query
	 		 aResourcPath = tmpRSP + strTmp; // need to be tested
	 		 return ;
	 	}
	 	else if (aResourcPath[i] == '%') //example file%3Ahg
	 	{
	 		c = aResourcPath[i + 3];
	 		aResourcPath[i + 3] = 0;
	 		str = aResourcPath.c_str() + 1 + i;
	 		// str now will be 3A in the above example
	 		tmpRSP.push_back(strtol(str, NULL, 16));
	 		aResourcPath[i + 3] = c; // return the previous character that was at this spot
	 		i += 2;
	 	}
	 	else
	 		tmpRSP.push_back(aResourcPath[i]);
	 }
	 aResourcPath = tmpRSP;
}

Response	*Request::AddToRequest(char *str, int size)
{
	endStr = str + size;
	Restmp = NULL;
	if (status == -1)
	{
		Restmp = StartLineParsing(&str, size);
		if (Restmp)
			return Restmp;
	}
	if (!status)
	{
		Restmp = ProcessHeaders(&str, size);
		if (Restmp)
			return Restmp;
	}
	if (status == 1)
	{
		if (method != POST)
			return HandleRequest(*this);
		Restmp = ReserveSpaceForBody();
		if (Restmp)
			return Restmp;
		if (ProcessBody(str, endStr - str))
		{
			if (totalRead > max_client_size) //// test this later
				return ResGen.getErrorResponse(comServerIndex, PAYLOAD_TOO_LARGE_STATUS_CODE); // test this later
			// bodyFileObj.close(); //changed
			bodyFileObj.seekg(0);
			return HandleRequest(*this); // this is here means request is done
		}
		if (totalRead > max_client_size) //// test this later
			return ResGen.getErrorResponse(comServerIndex, PAYLOAD_TOO_LARGE_STATUS_CODE); // test this later
	}
	return NULL; // request not  done yet
}

Response			*Request::ReserveSpaceForBody()
{
	if (!booltmp)
	{
		max_client_size = ServI[comServerIndex].whichServer(aHostName).getClientMaxBodySize(); // test here
		booltmp = true;
		// here make sure that this branch will only be excuted once
		if (isChuncked == 1)
		{
			fillFileName(connFD, fileName);
			bodyFileObj.open(fileName, std::fstream::out | std::fstream::binary); // creating file
			bodyFileObj.close();
			bodyFileObj.open(fileName, std::fstream::out | std::fstream::in | std::fstream::binary);
		}
		else if (bodySize != -1)
		{
			if (bodySize){
				totalRead = bodySize; // added
				if (bodySize > max_client_size)  // to be checked later
					return ResGen.getErrorResponse(comServerIndex, PAYLOAD_TOO_LARGE_STATUS_CODE); // keep eye on this line maybe i will change according to  defalut.conf file
				if (bodySize < MAX_BODY_SWITCH)
					bodyString.reserve(bodySize);
				else {
					fillFileName(connFD, fileName);
					bodyFileObj.open(fileName, std::fstream::out | std::fstream::binary); // creating file
					bodyFileObj.close();
					bodyFileObj.open(fileName, std::fstream::out | std::fstream::in | std::fstream::binary);
				}
			}
		}
		else
			return ResGen.getErrorResponse(comServerIndex, LENGTH_REQUIRED_STATUS_CODE);
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
		bodyString.insert(hasBeenRead, str, size);
		hasBeenRead += size;
		return false;
	}
	// i am not adding to hasBeenRead here bec i do not care any  more at this point
	bodyString.insert(hasBeenRead, str, bodySize - hasBeenRead);
	return true;
}

bool	Request::BodyFileCase(char *str, long size)
{
	if (hasBeenRead + size < bodySize)
	{
		InsertCharToFile(str, size);
		hasBeenRead += size;
		return false;
	}
	// i am not adding to hasBeenRead here bec i do not care any  more at this point
	InsertCharToFile(str, bodySize - hasBeenRead);
	return true;
}

void	Request::InsertCharToFile(char *str, long size)
{
	for (size_t i = 0; i < size; ++i)
		bodyFileObj << str[i];
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
				totalRead += bodySize; // added
				if (bodySize == 0)
					return true; // which means that the body is done dealing with
				tmpStr.clear();
				str += tmp; // MOVING str after "\r\n"
				size -= tmp;
				chunkedBodyState = false;
				hasBeenRead = 0;
			}
		}
		if (!chunkedBodyState)
		{
			if (BodyFileCase(str, size)) 
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
Response		*Request::ProcessHeaders(char **str, int size)
{
	//  if headers are done return NULL ans set status to 1
	// return NULL if headers are not done and set status to 0
	// maybe i will set -1 if error accured but i will remove this line later
	bool	opfold;
	while (*str)
	{
		status = ProcessOneLine(*str, size);
		if (IsHeadersDone(str))
		{
			if (aHostName.empty() && version == true)
				return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE); // HOST header does not exist
			status = 1;
			return NULL;
		}
		if (status != -1)
		{
			Restmp = FirstSecondFromHeaderLine(opfold); //"key :g"
			if (Restmp)
				return Restmp;
			if (!opfold)
			{
				Restmp = TakeInfoFromHeaders(str); // redefntion of host header
				if (Restmp)
					return Restmp;
			}
			else if (aHeaders.size())
			{
				// by the way this entire branch has not been tested yet
				std::string &opStr = aHeaders.back().second;
				opStr += ", ";
				opStr += first;
				(*str) += tmpStr.size();
				tmpStr.clear();
			}
			else
				return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE);
		}
		else
			break ;
	}
	status = 0;
	return NULL; // means headers not done yet
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

Response	*Request::TakeInfoFromHeaders(char **str)
{
	// this function will collect some informmation from header
	// for in Request needed informmation like Content-Length
	// or for respone info like Host
	if (str_cmp(first, "Host"))
	{
		if (aHostName.empty())
		{
			char *s;
			int tmp = IndexOf(second, ':'); // added
			if (tmp != -1) //added
			{
				s = const_cast<char *>(second);
				s[tmp] = 0; //added
				portStr = std::string(s + tmp + 1);
			}
			aHostName = second;
			if (tmp != -1)
				s[tmp] = ':';
		}
		else
			return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE);
	}
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
	return NULL;
}


Response	*Request::FirstSecondFromHeaderLine(bool &opfold)
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
		return NULL;
	}
	tmpStr[tmp] = 0;
	if (tmpStr[tmp - 1] == ' ')
		return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE);
	//second = first + tmp + 1;
	second = remove_speces_at_end_start(const_cast<char *>(first + tmp + 1), !strcmp(first, "Cookie"));

	// IF YOU NEED TO CHECK THE SECOND VALUE OF HEADER TO GENRATE ERROR CHECK AT THIS LINE
	return NULL;
}

const char	*remove_speces_at_end_start(char *str, bool co)
{
	int len = strlen(str) - 1;
	while (*str == ' ')
		++str;
	while (str[len] == ' ')
		--len;
	if (str[len  + 1] == ' ')
		str[len + 1] = 0;
	return str;
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
		return ResGen.getErrorResponse(comServerIndex, NOT_IMPLEMENTED_STATUS_CODE);
	if (method == UKNOWNMETHOD)
		return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE);
	// i need now to get resource path
	InitRecoursePath(aResourcPath, tmpStr, start);
	ResourseDecoding(); // i did not insert this function in InitRecoursePath bec it's not member of the Request class
	start = GetHttpVersion(tmpStr, start);
	if (start == HTTP_VERSION_NOT_SUPPORTED)
		return ResGen.getErrorResponse(comServerIndex, HTTP_VERSION_NOT_SUPPORTED_STATUS_CODE);
	if (start == SYNTAX_ERROR)
		return ResGen.getErrorResponse(comServerIndex, SYNTAX_STATUS_CODE);
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
    int i = 6; // this number is dependent on what is already in str variable

    while (num != 0)
    {
        int rem = num % 10;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/10;
    }
    str[i] = '\0';
}


// down just debug functions remove then when you are done

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

//////////////////////// getters

int Request::getCommonServerIndex()const
{
	return comServerIndex;
}

int Request::getMethod() const
{
	return method;
}

const std::string& Request::getResource()const
{
	return aResourcPath;
}

bool Request::getVersion() const
{
	return version;
}

const std::string& Request::getHost() const
{
	return aHostName;
}

const std::string& Request::getBody() const
{
	return bodyString;
}

const std::fstream& Request::getBodyFile() const
{
	return bodyFileObj;
}


bool Request::isBodyStr() const
{
	return bodyString.size();
}

const std::pair<std::string, in_port_t>	Request::GetClientInfo() const
{
	return client_info;
}


long	Request::getBodySize() const
{
	return totalRead;
}

const std::string		&Request::getPortStr() const
{
	return portStr;
}
