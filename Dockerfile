From ubuntu:16.04

RUN apt update &&\
    apt install -y  build-essential \
                    curl \
                    fuse \
                    fuse2fs \
                    gcc \
                    git \
                    libfuse-dev \
                    libfuse2 \
                    make \
                    pkg-config