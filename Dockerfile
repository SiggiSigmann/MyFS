#Docker version 18.06.1-ce, build 67f9a391
From ubuntu:16.04

RUN apt-get update &&\
    apt-get install build-essential=12.1ubuntu2 \
                    fuse=2.9.4-1ubuntu3.1 \
                    gcc=4:5.3.1-1ubuntu1 \
                    libfuse-dev=2.9.4-1ubuntu3.1 \
                    libfuse2=2.9.4-1ubuntu3.1 \
                    make=4.1-6 \
                    pkg-config=0.29.1-0ubuntu1 \
                    vim-common \
                    -y 
#add Node
RUN apt-get install --yes curl
RUN curl -sL https://deb.nodesource.com/setup_10.x | bash
RUN apt-get install nodejs -y
RUN echo 'alias runtest="cd /MyFS/fuseTests && npm test"' >> ~/.bashrc


COPY CMakeLists.txt Makefile /MyFS/
COPY includes /MyFS/includes/
COPY src /MyFS/src
COPY unittests /MyFS/unittests
COPY startmount.sh /MyFS/startmount.sh
COPY fuseTests /MyFS/fuseTests

WORKDIR /MyFS/

CMD make && ./unittest
