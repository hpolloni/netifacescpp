# Copyright (c) 2019 - Herman Polloni.
# This code is licensed under MIT license (see LICENSE for details)

# This is a convenient Dockerfile just to do some spot checking on non-Linux platform.
FROM ubuntu:18.04

RUN apt-get -y update
RUN apt-get -y install build-essential
RUN apt-get -y install cmake

RUN mkdir -p /opt/netifacescpp/linux_build

COPY . /opt/netifacescpp

WORKDIR /opt/netifacescpp/linux_build
RUN cmake ..
RUN make
RUN ./list_all

CMD ["bash"]