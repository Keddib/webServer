FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -yq curl telnet clang git vim make php-cgi php php-mysql mysql-server

RUN curl -O https://wordpress.org/latest.tar.gz && tar xzvf latest.tar.gz

RUN mv wordpress /var/www/ && rm /var/www/wordpress/wp-config-sample.php

#COPY wp-config.php /var/www/wordpress/

#mysql -u root -e "CREATE DATABASE wordpress;CREATE USER 'wp-user'@'%' IDENTIFIED BY 'wp-pass';GRANT ALL PRIVILEGES ON *.* TO 'wp-user'@'%';FLUSH PRIVILEGES;"

#CREATE DATABASE wordpress;
#CREATE USER 'wp-user'@'%' IDENTIFIED BY 'wp-pass';
#GRANT ALL PRIVILEGES ON *.* TO 'root'@'%';
#FLUSH PRIVILEGES;

WORKDIR /APP/

ENTRYPOINT bash
