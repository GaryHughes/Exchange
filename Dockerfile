FROM swift:latest
RUN apt-get -y update
RUN apt-get -y install wget apt-utils
RUN wget -q https://packages.microsoft.com/config/ubuntu/18.04/packages-microsoft-prod.deb -O packages-microsoft-prod.deb
RUN dpkg -i packages-microsoft-prod.deb
RUN apt-get -y update
RUN apt-get -y install apt-transport-https
RUN apt-get -y update
RUN apt-get -y install dotnet-sdk-3.0
RUN apt-get -y update && apt-get install -y make g++ clang++-7 libboost-all-dev golang 