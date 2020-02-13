#!/bin/bash

#echo "1 for armbian armhf"
echo "this script will fail if you have not setup docker with qemu"
#read -p "what platform would you like to build?: " PLATFORM 

#if [ $PLATFORM == "1" ]; then
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
docker build -f builds/Dockerfile.armhfbuster -t quserv:armhf-buster .
CONTAINER=$(docker create quserv:armhf-buster .)
docker cp ${CONTAINER}:/opt/qusbipserver.deb qusbipserver.deb
docker rm ${CONTAINER}
#else
#  echo "platform not found"
#fi
