NAME		= 	server

SRCS 		=	*.cpp

OBJ			=	*.o

GO			=	clang++ -Wall -Wextra -Werror #-fsanitize=address -g3

all:	$(NAME)

$(NAME): $(SRCS)
		@$(GO) -c $(SRCS)
		@$(GO) -o $(NAME) $(OBJ)
		@rm -rf $(OBJ)

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
