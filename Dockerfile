FROM ubuntu:20.04 as build

WORKDIR /opt/digitalbits-core

COPY . .
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install -y git build-essential alien pkg-config \
          autoconf automake libtool bison flex libpq-dev libunwind-dev \
          parallel gcc-8 g++-8 cpp-8 unzip curl pandoc clang \
          zip unzip tar wget gpg

# install latest cmake from Kitware APT Repository
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null && \
    echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null && \
    apt-get update && \
    rm /usr/share/keyrings/kitware-archive-keyring.gpg && \
    apt-get install -y kitware-archive-keyring && \
    apt-get install -y cmake

RUN ./vcpkg/bootstrap-vcpkg.sh && \
    ./vcpkg/vcpkg install "aws-sdk-cpp[secretsmanager]:x64-linux" --recurse && \
    ./vcpkg/vcpkg install "zlib:x64-linux"

RUN export CC=clang && export CXX=clang++ && \
    mkdir build && cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && \
    cd build && make -j`nproc` && make install

# update libraries to be copied with the executable
FROM ubuntu:20.04
RUN apt-get update && apt-get install libunwind8 curl libpq-dev -y && rm -rf /var/lib/apt/lists/* 
COPY --from=build /usr/local/lib/libmedida.so /usr/local/lib/
COPY --from=build /usr/local/bin/digitalbits-core /usr/local/bin/
ENTRYPOINT [ "/usr/local/bin/digitalbits-core" ]