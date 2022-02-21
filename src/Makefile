NAME	= Webserv

GO		= g++ -std=c++98 -Wall -Wextra -Werror  -ggdb3 -fsanitize=address

OFILES	= CommonServers.o ErrorGen.o Location.o ServersInterface.o requset.o\
		utils.o ConfigParser.o HelperFunctions.o Response.o VirtualServer.o manage-request.o\
		response-wrapper.o ReqHandler.o main.o HandleDirectory.o CGI.o

ODIR=obj

OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))

DEPS = MACROS.hpp configuration/*.hpp HandleRequest/*.hpp HandleRequest/CGI/*.hpp\
	start-servers/*.hpp

vpath %.cpp configuration HandleRequest HandleRequest/utils HandleRequest/CGI start-servers

all: $(NAME)

$(NAME): $(OBJ)
	$(GO) $(CFLAGS) -o $(NAME) $(OBJ)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

re: fclean all

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)


run: start-servers/manage-request.cpp
	g++ -ggdb3 -O0   obj/*.o start-servers/manage-request.cpp   start-servers/requset.cpp start-servers/response-wrapper.cpp -o run

cgi: HandleRequest/CGI/CGI.cpp start-servers/requset.cpp 
	g++ -ggdb3 obj/*.o HandleRequest/CGI/CGI.cpp  HandleRequest/ReqHandler.cpp   -o cgi

.PHONY: clean fclean re all
