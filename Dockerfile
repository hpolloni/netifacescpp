# This is a convenient Dockerfile just to do some spot checking on non-Linux platform.
FROM ubuntu:18.04

RUN apt-get -y update
RUN apt-get -y install build-essential
RUN apt-get -y install cmake
RUN apt-get -y install vim net-tools

RUN mkdir -p /opt/netifacescpp/linux_build

COPY . /opt/netifacescpp

WORKDIR /opt/netifacescpp/linux_build
RUN cmake ..
RUN make
RUN ./list_all

CMD ["bash"]