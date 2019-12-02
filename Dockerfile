FROM mcr.microsoft.com/dotnet/core/sdk:3.0
RUN apt-get -y update && apt-get install -y make g++ clang++-7 libboost-all-dev golang