FROM ubuntu:latest

RUN apt-get update && apt-get install -y gcc

COPY ../ps.c /ps.c
RUN gcc /ps.c -o /ps

CMD /ps
