Installation Instructions

==================
## Prebuild software
DigitalBits.io publishes software packages to the cloudsmith.io repository https://cloudsmith.io/~xdb-foundation/repos/digitalbits-core/packages/

Packages available as:
   - .deb and .rpm packages and 
   - prebuild binaries for windows, linux and macos
   - docker image.  

digitalbits-core 1.0.46 version

## Ubuntu

1. Configure digitalbits-core repository from cloudsmith.io:

    curl -1sLf 'https://dl.cloudsmith.io/public/xdb-foundation/digitalbits-core/setup.deb.sh' | sudo -E bash

2. Install digitalbits-core package:

    sudo apt-get install digitalbits-core=1.0.46


## RPM 
1. Configure digitalbits-core repository from cloudsmith.io:

    curl -1sLf 'https://dl.cloudsmith.io/public/xdb-foundation/digitalbits-core/setup.rpm.sh' | sudo -E bash

2. Install digitalbits-core package:

    sudo yum install digitalbits-core-1.0.46-1.x86_64

## Raw binaries

- MacOS

    curl -O 'https://dl.cloudsmith.io/public/xdb-foundation/digitalbits-core/raw/files/digitalbits-core_1.0.46_darwin-amd64.tar.gz'

- Linux

    curl -O 'https://dl.cloudsmith.io/public/xdb-foundation/digitalbits-core/raw/files/digitalbits-core_1.0.46_linux-amd64.tar.gz'

- Windows

    curl -O 'https://dl.cloudsmith.io/public/xdb-foundation/digitalbits-core/raw/files/digitalbits-core_1.0.46_windows-amd64.tar.gz'


## Docker image

    docker pull docker.cloudsmith.io/xdb-foundation/digitalbits-core/digitalbits-core:1.0.46

==================
## From sources

These are intructions for building digitalbits-core from source. For a potentially quicker set up we also have digitalbits-core in a docker container:

Use docker to build the app:
```shell
docker build -t digitalbits-core:latest .
```
and then you can check the app by running:
```shell
docker run --rm digitalbits-core:latest digitalbits-core version
```

## Picking a version to run

Branches are organized in the following way:

| branch name | description | quality bar |
| ----------- | ----------- | ----------- |
| master      | development branch | all unit tests passing |
| testnet     | version deployed to testnet | acceptance tests passing |
| prod        | version currently deployed on the live network | no recall class issue found in testnet and staging |

For convenience, we also keep a record in the form of release tags of the
 versions that make it to production:
 * pre-releases are versions that get deployed to testnet
 * releases are versions that made it all the way in prod

When running a node, the best bet is to go with the latest release.

## Build Dependencies

- `clang` >= 3.5 or `g++` >= 4.9
- `pkg-config`
- `bison` and `flex`
- `libpq-dev` unless you `./configure --disable-postgres` in the build step below.
- 64-bit system
- `clang-format-5.0` (for `make format` to work)

### Ubuntu 16.04

    # sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    # sudo apt-get update
    # sudo apt-get install git build-essential pkg-config autoconf automake libtool bison flex libpq-dev clang++-3.5 gcc-4.9 g++-4.9 cpp-4.9

In order to make changes, you'll need to install the proper version of clang-format (you may have to follow instructions on https://apt.llvm.org/ )
    # sudo apt-get install clang-format-5.0

See [installing gcc 4.9 on ubuntu 16.04](http://askubuntu.com/questions/428198/getting-installing-gcc-g-4-9-on-ubuntu)

Additional, for proper documentation generation (man page), pandoc is needed:
    # sudo apt-get install pandoc

### OS X
When building on OSX, here's some dependencies you'll need:
- Install xcode
- Install homebrew
- brew install libsodium
- brew install libtool
- brew install automake
- brew install pkg-config
- brew install libpqxx *(If ./configure later complains about libpq missing, try PKG_CONFIG_PATH='/usr/local/lib/pkgconfig')*

### Windows
See [INSTALL-Windows.md](INSTALL-Windows.md)

## Basic Installation

- `git clone https://github.com/xdbfoundation/digitalbits`
- `cd digitalbits`
- `git submodule init`
- `git submodule update`
- Type `./autogen.sh`.
- Type `./configure`   *(If configure complains about compiler versions, try `CXX=clang-3.5 ./configure` or `CXX=g++-4.9 ./configure` or similar, depending on your compiler.)*
- Type `make` or `make -j` (for aggressive parallel build)
- Type `make check` to run tests.
- Type `make install` to install.



















