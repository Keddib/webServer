#ifndef	__MACROS__
#define __MACROS__


	// check if the user defined an error page for the error accured
#define ERR400 "<!DOCTYPE html>\n<html>\n<head><title>400 Bad Request</title></head>\n<body><center><h1>400 Bad Request</h1></center><hr><center>webserv/1.0</center></body>\n</html>"
#define ERR501 "<!DOCTYPE html>\n<html>\n<head><title>501 not implemented</title></head>\n<body><center><h1>501 not implemented</h1></center><hr><center>webserv/1.0</center></body>\n</html>"
#define ERR405 "<!DOCTYPE html>\n<html>\n<head><title>405 not allowed</title></head>\n<body><center><h1>405 not allowed</h1></center><hr><center>webserv/1.0</center></body>\n</html>"

#define	MAX_BACK_LOG	20
#define ERROR_ROOT "/Users/keddib/Desktop/Keddib/42Projects/WebServ/www/error/"
#define CONFIG_FILE "./src/config/default.conf"
#define DEFAULT_ROOT "/Users/keddib/Desktop/Keddib/42Projects/WebServ/www/"
#define ERROR_ROOT "/Users/keddib/Desktop/Keddib/42Projects/WebServ/www/error/"
#define D_PORT 8000
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
#define NONE -1

#define	BUFFER_SIZE	1024 // make sure that this number is fiting in int type
#define GET 15
#define POST 16
#define DELETE 17
#define UKNOWNMETHOD -1
#define GET_STR "GET"
#define POST_STR "POST"
#define DELETE_STR "DELETE"
#define LONGEST_METHOD 7 // +1 is neccessary
#define MAX_BODY_SWITCH 100 // this will be used if body of post Request is under this MAX_BODY_SWITCH i will store it as string if bigger i will store it as file

#endif
