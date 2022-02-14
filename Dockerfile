FROM ubuntu:latest

RUN apt update && apt install -y clang git vim make

ENTRYPOINT bash
