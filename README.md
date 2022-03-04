# webserv
## Description

webserv is a HTTP/1.1 server written in C++98. It must be conditionnal compliant with RFC 7230 to 7235.

## Usage

```shell
# Compile the sources
make
```
```shell
# Run the server
./webserv [options] [config_file]
```
#### Options
- --help```        : Display help text
- -t```            : Test the config file, dump it and exit
- [config_file]``` : run with this config_file
- webserv```       : run with default condig
```
$ ./webserv --help

Usage: webserv [options] [config_file]

Options:
  --help         : this help text
  -t             : test config and exit
  [config_file]  : run with this config file
  webserv        : run with defualt config
```

## Configuration File

### Directives

#### location : ```location uri;```

sets configuration depending on the given uri.

#### port : [port number] ```listen port;```
#### host : [address] ```listen adress;```

bind the given port. if no address is given, binds 0.0.0.0. if no port is given, binds 80.

#### server_name : ```server_name name;```

sets name of a virtual server.

#### root : ```root path;```

sets the directory for requests.

#### error_page : ```error_page code ... uri;```

defines the URI that will be shown for the specified errors.

#### upload_loc : ```upload directory;```

defines a directory to upload file.

#### auto_index : ```auto_index on | off;``` (default off)

enables or disables the directory listing output.

#### index : ```index file ...;```

#### client_max_body_size : ```client_max_body_size [size in bytes];```

Limit client body size.

#### return : ```return code uri;```

Define a HTTP redirection.

#### accepted_methods : ```accepted_methods GET POST ...;```

Define a list of accepted HTTP methods for the route.

#### cgi_read_timeout : ```cgi_read_timeout [time in seconds];```

set CGI time-out

#### CGI : ```cgi extension cgi_path;```

defines a CGI binary that will be executed for the given extension.

### Example

```
server
	port 80
	server_name localhost
	client_max_body_size 100000
	location /
		index index.html index.htm
		accepted_methods GET POST
		CGI .py /usr/bin/python
```


## Authors

* [keddib](https://github.com/keddib)
* [mel-ghar](https://github.com/Gharbaoui)
