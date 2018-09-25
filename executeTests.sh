#/bin/sh
docker build -q -t test-machine .
docker run test-machine