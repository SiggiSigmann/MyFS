#/bin/sh
docker build -q -t test-machine . > /dev/null
docker run --name test-container test-machine
docker rm test-container > /dev/null