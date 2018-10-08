#Docker version 18.06.1-ce, build 67f9a391
From ubuntu:16.04

RUN apt-get update &&\
    apt-get install build-essential=12.1ubuntu2 \
                    fuse=2.9.4-1ubuntu3.1 \
                    gcc=4:5.3.1-1ubuntu1 \
                    git=1:2.7.4-0ubuntu1.4 \
                    libfuse-dev=2.9.4-1ubuntu3.1 \
                    libfuse2=2.9.4-1ubuntu3.1 \
                    make=4.1-6 \
                    pkg-config=0.29.1-0ubuntu1 \
                    -y

COPY CMakeLists.txt Makefile /MyFS/
COPY includes /MyFS/includes/
COPY src /MyFS/src
COPY unittests /MyFS/unittests

WORKDIR /MyFS/
CMD rm *.o
CMD make && ./unittest