if(MSVC)
    set(SODIUM_STATIC_LIB_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Debug/v143/static/libsodium.lib)
    set(SODIUM_STATIC_LIB_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64/Release/v143/static/libsodium.lib)

    include_external_msproject(libsodium ${CMAKE_CURRENT_SOURCE_DIR}/builds/msvc/vs2019/libsodium/libsodium.vcxproj)
    set_target_properties(libsodium PROPERTIES
        MAP_IMPORTED_CONFIG_RELEASE ReleaseLIB
        MAP_IMPORTED_CONFIG_DEBUG DebugLIB)

    add_library(sodium STATIC IMPORTED GLOBAL)
    add_dependencies(sodium libsodium)

    set_target_properties(sodium PROPERTIES
        IMPORTED_LOCATION_RELEASE "${SODIUM_STATIC_LIB_RELEASE}"
        IMPORTED_LOCATION_DEBUG "${SODIUM_STATIC_LIB_DEBUG}"
        IMPORTED_CONFIGURATIONS "RELEASE;DEBUG")
else()
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
endif()
