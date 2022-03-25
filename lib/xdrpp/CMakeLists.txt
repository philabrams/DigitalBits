set(XDRPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(XDRPP_BIN ${CMAKE_CURRENT_BINARY_DIR}/xdrpp)
set(XDRPP_STATIC_LIB ${XDRPP_BIN}/lib/libxdrpp.a)
set(XDRPP_INCLUDES ${XDRPP_BIN}/include)

file(MAKE_DIRECTORY ${XDRPP_INCLUDES})

ExternalProject_Add(libxdrpp
    PREFIX ${XDRPP_BIN}
    SOURCE_DIR ${XDRPP_DIR}
    DOWNLOAD_COMMAND cd ${XDRPP_DIR} && ${XDRPP_DIR}/autogen.sh
    CONFIGURE_COMMAND ${XDRPP_DIR}/configure --srcdir=${XDRPP_DIR} --prefix=${XDRPP_BIN} --enable-static=yes --disable-shared CC=gcc CXX=g++
    BUILD_COMMAND ${MAKE}
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${XDRPP_STATIC_LIB})

add_library(xdrpp STATIC IMPORTED GLOBAL)

add_dependencies(xdrpp libxdrpp)

set_target_properties(xdrpp PROPERTIES IMPORTED_LOCATION ${XDRPP_STATIC_LIB})
set_target_properties(xdrpp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${XDRPP_INCLUDES})
