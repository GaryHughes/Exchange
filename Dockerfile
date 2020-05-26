FROM mcr.microsoft.com/dotnet/core/sdk:3.1-focal

RUN apt-get -y update && apt-get install -y \
    make \
    libboost-all-dev \
    golang \
    g++ \
    libicu-dev \
    wget \
    libtinfo5 \
    libncurses5 \
    ant \
    openjdk-11-jdk \
    vim \
    python3-pip
    
#
# These are for the Cython builds
#
RUN pip3 install cython setuptools

#
# Clang
#
RUN curl -SL https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/clang+llvm-10.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar --strip-components 1 -xJC /usr/local
   
#
# Swift
#
RUN wget -q https://swift.org/builds/swift-5.1.2-release/ubuntu1804/swift-5.1.2-RELEASE/swift-5.1.2-RELEASE-ubuntu18.04.tar.gz \
    && tar zxf swift-5.1.2-RELEASE-ubuntu18.04.tar.gz --strip-components=1 -C /

