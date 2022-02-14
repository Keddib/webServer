#!/bin/sh

#build image
docker build -t webserv-img .

#run container
docker run -it --publish 80-85:80-85 -v $(PWD):/APP webserv-img
