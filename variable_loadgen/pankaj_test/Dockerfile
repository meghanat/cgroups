FROM ubuntu:14.04
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
ENV DEBIAN_FRONTEND noninteractive
RUN apt-get update
RUN apt-get -y install gcc mono-mcs
RUN gcc -o myapp loadgen_r4.c
CMD ["touch hugeinp1.txt"]
