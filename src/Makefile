NAME	= Webserv

GO		= clang++ -std=c++98 -Wall -Wextra -Werror  -ggdb3 -fsanitize=address

OFILES	= CommonServers.o ResGenerator.o Location.o ServersInterface.o requset.o\
		StringManipulation.o HandleCGIresponse.o HandleFile.o ConfigParser.o HelperFunctions.o Response.o VirtualServer.o manage-request.o\
		response-wrapper.o ReqHandler.o  HandleRequest.o main.o HandleDirectory.o

ODIR=obj

OBJ = $(patsubst %,$(ODIR)/%,$(OFILES))

DEPS = MACROS.hpp Includes.hpp configuration/*.hpp Response/*.hpp start-servers/*.hpp

vpath %.cpp configuration Response start-servers utils

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
