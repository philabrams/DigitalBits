set(SODIUM_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SODIUM_BIN ${CMAKE_CURRENT_BINARY_DIR}/libsodium)
set(SODIUM_STATIC_LIB ${SODIUM_BIN}/lib/libsodium.a)
set(SODIUM_INCLUDES ${SODIUM_BIN}/include)

file(MAKE_DIRECTORY ${SODIUM_INCLUDES})

ExternalProject_Add(libsodium
    PREFIX ${SODIUM_BIN}
    SOURCE_DIR ${SODIUM_DIR}
    DOWNLOAD_COMMAND cd ${SODIUM_DIR} && ${SODIUM_DIR}/autogen.sh
    CONFIGURE_COMMAND ${SODIUM_DIR}/configure --srcdir=${SODIUM_DIR} --prefix=${SODIUM_BIN} --enable-static=yes --disable-shared
    BUILD_COMMAND ${MAKE}
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${SODIUM_STATIC_LIB})

add_library(sodium STATIC IMPORTED GLOBAL)

add_dependencies(sodium libsodium)

set_target_properties(sodium PROPERTIES IMPORTED_LOCATION ${SODIUM_STATIC_LIB})
set_target_properties(sodium PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${SODIUM_INCLUDES})
