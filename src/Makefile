NAME		= 	Webserv

SRCS 		=	configuration/ConfigParser.cpp\
				configuration/ServersInterface.cpp\
				configuration/CommonServers.cpp\
				configuration/VirtualServer.cpp\
				configuration/Location.cpp\
				configuration/utils.cpp\
				HandleRequest/ReqHandler.cpp\
				HandleRequest/Response.cpp\
				HandleRequest/ErrorGen.cpp\
				HandleRequest/utils/HelperFunctions.cpp\
				HandleRequest/utils/HandleDirectory.cpp\
				start-servers/requset.cpp\
				start-servers/response-wrapper.cpp\
				start-servers/manage-request.cpp\
				start-servers/main.cpp


OBJ			=	obj/*.o

GO			=	clang++ -std=c++98 -Wall -Wextra -Werror #-g3 #-fsanitize=address

all:	$(NAME)
$(NAME): $(SRCS)
		$(GO) -c $(SRCS)
		mv *.o obj
		$(GO) -o $(NAME) $(OBJ)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
