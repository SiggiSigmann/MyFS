#/bin/sh
docker build -t test-machine . && docker run -it --cap-add=SYS_ADMIN --privileged test-machine bash
