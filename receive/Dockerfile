FROM ubuntu:latest

RUN apt-get update && apt-get install -y gcc

COPY pr.c /pr.c
RUN gcc /pr.c -o /pr

CMD /pr
