FROM ubuntu:18.04
RUN apt-get -y update && apt-get install -y make g++ libboost-all-dev