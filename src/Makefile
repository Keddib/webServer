NAME	= Webserv

GO		= clang++ -std=c++98 -Wall -Wextra -Werror #-g3 #-fsanitize=address

OFILES	= CommonServers.o ErrorGen.o Location.o ServersInterface.o requset.o\
		utils.o ConfigParser.o HelperFunctions.o Response.o VirtualServer.o manage-request.o\
		response-wrapper.o ReqHandler.o main.o HandleDirectory.o

ODIR=obj

OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))

DEPS = MACROS.hpp configuration/*.hpp HandleRequest/*.hpp start-servers/*.hpp

vpath %.cpp configuration HandleRequest HandleRequest/utils start-servers

all: $(NAME)

$(NAME): $(OBJ)
	$(GO) -o $(NAME) $(OBJ)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

re: fclean all

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

.PHONY: clean fclean re all
