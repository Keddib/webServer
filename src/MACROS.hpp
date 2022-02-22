#ifndef	__MACROS__
#define __MACROS__


	// check if the user defined an error page for the error accured


#define HTML1 "<!DOCTYPE html>\n<html>\n<head><title>"
#define HTML2 "</title></head>\n<body><center><h1>"
#define HTML3 "</h1></center><hr><center>webserv/1.1</center></body>\n</html>"

#define Dir1 "<!DOCTYPE html><head><title>Index of</title></head><body><h1>Index of "
#define Dir2 "</pre><hr></body></html>"

#define	MAX_BACK_LOG	100
#define CONFIG_FILE "./config/default.conf"
#define DEFAULT_ROOT "/APP/www"
#define ERROR_ROOT ""
#define D_PORT 80
#define HOST 1
#define PORT 2
#define S_NAME 3
#define CMBS 4
#define ERRPG 5
#define LOCATION 6
#define ROOT 7
#define INDEX 8
#define AUTO 9
#define ACCMETH 10
#define RET 11
#define UPLD 12
#define CGI 13
#define CGI_TO 18
#define NONE -1

#define	BUFFER_SIZE	4096 // make sure that this number is fiting in int type
#define GET 15
#define POST 16
#define DELETE 17
#define NOT_IMPLEMENTED 2
#define NOT_IMPLEMENTED_STATUS_CODE 501
#define UKNOWNMETHOD -1
#define SYNTAX_STATUS_CODE 400
#define SYNTAX_ERROR 1000 // this is positive because there exist some cases when i use unsigned type to store it
#define GET_STR "GET"
#define POST_STR "POST"
#define DELETE_STR "DELETE"

#define OPTIONS_STR "OPTIONS"
#define HEAD_STR "HEAD"
#define PUT_STR "PUT"
#define PATCH_STR "PATCH"

#define HTTP_VERSION_SUPPORTED 0
#define HTTP_VERSION_NOT_SUPPORTED 3
#define HTTP_VERSION_NOT_SUPPORTED_STATUS_CODE 505

#define LENGTH_REQUIRED_STATUS_CODE 411
#define CLIENT_MAX_BODY_SIZE 1048576 // this number is 1M
#define PAYLOAD_TOO_LARGE_STATUS_CODE 413

#define LONGEST_METHOD 7 // +1 is neccessary
#define MAX_BODY_SWITCH  0 //1048576 this will be used if body of post Request is under this MAX_BODY_SWITCH i will store it as string if bigger i will store it as file

#define RS_TMOUT 30
#define TIMEOUT 3600
#define EPOLL_RETRY 5

#define	READ	0
#define	WRITE	1


#endif
