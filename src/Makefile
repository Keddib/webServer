NAME	= Webserv

GO		= clang++ -std=c++98 -Wall -Wextra -Werror  #-ggdb3 -fsanitize=address

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


cgi: Response/HandleRequest.cpp Response/ReqHandler.cpp  Response/ResGenerator.cpp Response/Response.cpp utils/HandleCGIresponse.cpp
	g++ -ggdb3 ./obj/*.o Response/HandleRequest.cpp Response/ReqHandler.cpp  Response/ResGenerator.cpp Response/Response.cpp utils/HandleCGIresponse.cpp  -o cgi

.PHONY: clean fclean re all
