From ubuntu:16.04

RUN apt-get update &&\
    apt-get install build-essential \
                    curl \
                    fuse \
                    gcc \
                    git \
                    libfuse-dev \
                    libfuse2 \
                    make \
                    pkg-config \
                    -y

Run git clone https://github.com/hacker-h/MyFS