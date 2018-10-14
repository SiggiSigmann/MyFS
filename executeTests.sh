#/bin/sh

# Usage info
show_help() {
cat << EOF
Usage: ${0##*/} [option]
This shell script builds and runs a docker image provisioning a fuse file system and executing its unit tests.
For performance optimization the precompiled *.o files are passed into the container utilizing a volume by default.
There will be only few output by default.
options:
    -f          force. Rebuild the docker image freshly without caching, takes significantly longer
    -h          help. displays this help message and exit
    -v          verbose. Print lots debugging output.
recognized arguments:
EOF
printf '<%s>\n' "$@"
}

#logging function
log() {
if [ $verbose -ne 0 ]
then
echo "$1"
fi
}

#variables
verbose=0
volume="-v obj:/MyFS/obj "
suppressOutput="|& /dev/null"
quiet="-q"

#|& is syntactic sugar for 2>&1 |

#check if docker is installed
which docker |& /dev/null
if [ $? -ne 0 ]
then
    echo "Docker is not installed, aborting.." >&2
    exit
fi
#check if docker is ready to use
docker ps |& /dev/null
if [ $? -ne 0 ]
then
    echo "Docker is not running, aborting.." >&2
    exit
fi

#remove dead container if it exists silently
log "docker stop test-container"
eval "docker stop test-container ${suppressOutput}"
log "docker rm test-container ${suppressOutput}"
eval "docker rm test-container ${suppressOutput}"

while getopts fhv opt; do #watch for arguments f, h and v
    case $opt in
        h)
            show_help
            exit 0
            ;;
        v)  verbose=1 #enable logs
            suppressOutput="" #disable suppressed output by piping
            quiet="" #disable suppressed output by -q
            ;;
        f)  #omit the volume when force flag is set to force full recompilation
            volume=""
            ;;
        *) #for any invalid parameters -> show help
            show_help >&2
            exit 1
            ;;
    esac
done

#build the image silently
log "docker build -t test-image ."
eval "docker build ${quiet} -t test-image . ${suppressOutput}"

#run the container
#using the 'obj' dir as volume enables the caching of linking objects to save compile time
#when using docker toolbox the volume 'obj' is located on the disk of the virtual machine (linux) hosting the docker environment and will persist forever by default

log "docker run --name test-container ${volume} test-image"
docker run --name test-container ${volume} test-image

#remove the dead container silently
log "docker rm test-container ${suppressOutput}"
eval "docker rm test-container ${suppressOutput}"