#/bin/sh
#check if docker is available
docker ps > /dev/null
if [ $? -ne 0 ]
then
    echo "Docker is not running, aborting.."
    exit
fi


#build the image silently
docker build -q -t test-machine 

#run the container
#using the 'obj' dir as volume enables the caching of linking objects to save compile time
#on docker for windows the volume 'obj' is located on the disk of the virtual machine (linux) hosting the docker environment and will persist forever by default
docker run --name test-container -v obj:/MyFS/obj test-machine

#remove the dead container silently
docker rm test-container > /dev/null