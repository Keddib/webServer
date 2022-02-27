#to make things simple and easy to maintain :
# the server block will include only the following:
# 1- port portn
# 2- host adress
# 3- server_name example.com
# 4- error_page code filepath : for this we will support only 400 403 404 405
# 5- client_max_body_size bytes
# 6- location block.

# the location block will contain the following :
# 1- accepted_methods M1 M2 M3 ...
# 2- auto_index off/on
# 3- index example.html example.txt ...
# 4- root /path/to/serve/
# cgi_read_timeout sec
# 5- CGI .extn /cgi/path
# 6- return code URI
# 7- upload_loc /path/to/safe/files

example

server
	port 80
	server_name localhost
	client_max_body_size 100000
	location /
		index index.html index.htm
		accepted_methods GET POST
		CGI .py /usr/bin/python
