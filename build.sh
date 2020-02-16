#!/bin/bash

echo "this script will fail if you have not setup docker with qemu"
echo "1 for debian armhf"
echo "2 form debian arm64"
echo "3 form debian amd64"
read -p "what platform would you like to build?: " PLATFORM 

if [ $PLATFORM == "1" ]; then
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
docker build -f builds/Dockerfile.armhfbuster -t quserv:armhf-buster .
CONTAINER=$(docker create quserv:armhf-buster .)
docker cp ${CONTAINER}:/opt/qusbipserver.deb qusbipserver_armhf.deb
docker rm ${CONTAINER}
elif [ $PLATFORM == "2" ]; then
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
docker build -f builds/Dockerfile.arm64buster -t quserv:arm64-buster .
CONTAINER=$(docker create quserv:arm64-buster .)
docker cp ${CONTAINER}:/opt/qusbipserver.deb qusbipserver_arm64.deb
docker rm ${CONTAINER}
elif [ $PLATFORM == "3" ]; then
docker build -f builds/Dockerfile.amd64buster -t quserv:amd64-buster .
CONTAINER=$(docker create quserv:amd64-buster .)
docker cp ${CONTAINER}:/opt/qusbipserver.deb qusbipserver_amd64.deb
docker rm ${CONTAINER}
else
  echo "platform not found"
fi
