
FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -yq curl telnet clang git vim make php-cgi php php-mysql mysql-server python

RUN rm -rf /var/www/html

COPY examples/html /var/www/html

COPY examples/wordpress /var/www/wordpress

COPY examples/furn/ /var/www/furn

COPY examples/cgi-bin/ /var/www/cgi-bin

RUN mkdir /var/www/upload

#mysql -u root -e "CREATE DATABASE wordpress;CREATE USER 'wp-user'@'%' IDENTIFIED BY 'wp-pass';GRANT ALL PRIVILEGES ON *.* TO 'wp-user'@'%';FLUSH PRIVILEGES;"

WORKDIR /APP/

ENTRYPOINT bash
