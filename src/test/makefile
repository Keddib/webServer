NAME		= 	server

SRCS 		=	TimeServer.cpp StreamSocket.cpp

GO			=	clang++ -Wall -Wextra -Werror #-g3 #-fsanitize=address -g3

all:	$(NAME)

$(NAME): $(SRCS)
		@$(GO) $(SRCS) -o $(NAME)
		@rm -rf $(OBJ)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
