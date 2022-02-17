FROM ubuntu:latest

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -yq clang git vim make php-cgi php

ENTRYPOINT bash
