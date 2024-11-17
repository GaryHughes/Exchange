FROM  mcr.microsoft.com/dotnet/nightly/sdk:9.0-noble

RUN apt-get -y update && apt-get install -y \
    make \
    libboost-all-dev \
    golang \
    g++ \
    libicu-dev \
    wget \
    libncurses6 \
    ant \
    openjdk-11-jdk \
    vim \
    python3-pip \
    nodejs \
    npm \
    lsb-release \
    software-properties-common \
    gnupg

#
# Clang
#
#RUN curl -SL https://github.com/llvm/llvm-project/releases/download/llvmorg-18.1.8/clang+llvm-18.1.8-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar --strip-components 1 -xJC /usr/local
RUN wget -qO- https://apt.llvm.org/llvm.sh | bash -s -- 18 

#
# Swift
#
RUN wget -q https://download.swift.org/swift-6.0.2-release/ubuntu2404/swift-6.0.2-RELEASE/swift-6.0.2-RELEASE-ubuntu24.04.tar.gz \
    && tar zxf swift-6.0.2-RELEASE-ubuntu24.04.tar.gz --strip-components=1 -C /
