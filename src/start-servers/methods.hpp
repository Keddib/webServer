#ifndef __METHOD__
#define __METHOD__

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
