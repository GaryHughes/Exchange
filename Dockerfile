FROM swift:latest
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get -y -q update
RUN apt-get install -y -q apt-utils
RUN apt-get install -y -q wget 
RUN wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
RUN dpkg -i packages-microsoft-prod.deb
RUN apt-get -y -q update
RUN apt-get install -y -q apt-transport-https
RUN apt-get -y -q update
RUN apt-get -y -q install dotnet-sdk-3.0
RUN apt-get -y update && apt-get install -y make g++ clang++-7 libboost-all-dev golang 