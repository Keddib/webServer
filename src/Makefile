NAME		= 	Webserv

SRCS 		=	main.cpp \
				configuration/ConfigParser.cpp\
				configuration/ServersInterface.cpp\
				configuration/CommonServers.cpp\
				configuration/VirtualServer.cpp\
				configuration/Location.cpp\
				configuration/utils.cpp\
				HandleRequest/ReqHandler.cpp\
				HandleRequest/Response.cpp\
				HandleRequest/ErrorGen.cpp\
				HandleRequest/utils/HelperFunctions.cpp\
				start-servers/requset.cpp

OBJ			=	*.o

GO			=	clang++ -std=c++98 -Wall -Wextra -Werror -g3 #-fsanitize=address -g3

all:	$(NAME)

$(NAME): $(SRCS)
		@$(GO) -c $(SRCS)
		@$(GO) -o $(NAME) $(OBJ)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
