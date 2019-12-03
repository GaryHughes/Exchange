FROM mcr.microsoft.com/dotnet/core/sdk:3.0
RUN apt-get -y update && apt-get install -y make g++ libboost-all-dev golang clang libicu-dev wget libtinfo5
RUN wget -q https://swift.org/builds/swift-5.1.2-release/ubuntu1804/swift-5.1.2-RELEASE/swift-5.1.2-RELEASE-ubuntu18.04.tar.gz
RUN tar zxf swift-5.1.2-RELEASE-ubuntu18.04.tar.gz --strip-components=1 -C /
