#/bin/sh
docker build -t test-machine . && docker run -it test-machine bash
