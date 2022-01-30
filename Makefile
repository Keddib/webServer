NAME		= 	./start-servers/start

SRCS 		=	./start-servers/main.cpp ./configuration/ConfigParser.cpp ./configuration/VirtualServer.cpp ./configuration/utils.cpp ./configuration/ServersInterface.cpp ./configuration/CommonServers.cpp ./configuration/Location.cpp
GO			=	clang++ -ggdb3


all:	$(NAME)

$(NAME): $(SRCS)
		@$(GO) -o $(NAME) $(SRCS)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
