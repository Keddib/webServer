NAME		= 	Webserv

SRCS 		=	main.cpp \
				configuration/ConfigParser.cpp\
				configuration/ServersInterface.cpp\
				configuration/CommonServers.cpp\
				configuration/VirtualServer.cpp\
				configuration/Location.cpp\
				configuration/utils.cpp\
				HandleRequest/ReqHandler.cpp\
				HandleRequest/Request.cpp\
				HandleRequest/Response.cpp

OBJ			=	*.o

GO			=	clang++ -std=c++98 -Wall -Wextra -Werror #-fsanitize=address -g3

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
