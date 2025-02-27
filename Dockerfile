FROM ubuntu:20.04 as build

WORKDIR /opt/digitalbits-core

COPY . .
RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt-get update && \
    apt-get install git build-essential alien pkg-config \
          autoconf automake libtool bison flex libpq-dev libunwind-dev \
          parallel gcc-8 g++-8 cpp-8 unzip curl -y

RUN ./autogen.sh
RUN ./configure

# make -jN where N is amount of your cpu cores - 1
RUN make -j`nproc`
RUN make install

FROM ubuntu:20.04
RUN apt-get update && apt-get install libunwind8 curl libpq-dev -y && rm -rf /var/lib/apt/lists/* 
COPY --from=build /usr/local/bin/digitalbits-core /usr/local/bin/
ENTRYPOINT [ "/usr/local/bin/digitalbits-core" ]