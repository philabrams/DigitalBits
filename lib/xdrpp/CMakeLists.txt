if(MSVC)
    set(XDRPP_STATIC_LIB_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/x64/Release/xdrpp.lib)
    set(XDRPP_STATIC_LIB_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/x64/Debug/xdrpp.lib)
    set(XDRC_EXECUTABLE_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/x64/Release/xdrc.exe)
    set(XDRC_EXECUTABLE_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/x64/Debug/xdrc.exe)

    include_external_msproject(libxdrpp ${CMAKE_CURRENT_SOURCE_DIR}/msvc_xdrpp/xdrpp/xdrpp.vcxproj)
    include_external_msproject(xdrc_exe ${CMAKE_CURRENT_SOURCE_DIR}/msvc_xdrpp/xdrc/xdrc.vcxproj)

    add_library(xdrpp STATIC IMPORTED GLOBAL)
    add_dependencies(xdrpp libxdrpp)
    set_target_properties(xdrpp PROPERTIES
        IMPORTED_LOCATION "${XDRPP_STATIC_LIB_RELEASE}"
        IMPORTED_LOCATION_DEBUG "${XDRPP_STATIC_LIB_DEBUG}"
        IMPORTED_CONFIGURATIONS "RELEASE;DEBUG")

    add_executable(xdrc IMPORTED GLOBAL)
    add_dependencies(xdrc xdrc_exe)
    set_target_properties(xdrc PROPERTIES
        IMPORTED_LOCATION "${XDRC_EXECUTABLE_RELEASE}"
        IMPORTED_LOCATION_DEBUG "${XDRC_EXECUTABLE_DEBUG}"
        IMPORTED_CONFIGURATIONS "RELEASE;DEBUG")
else()
    set(XDRPP_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    set(XDRPP_BIN ${CMAKE_CURRENT_BINARY_DIR}/xdrpp)
    set(XDRPP_STATIC_LIB ${XDRPP_BIN}/lib/libxdrpp.a)
    set(XDRPP_INCLUDES ${XDRPP_BIN}/include)
    set(XDRC_EXECUTABLE ${XDRPP_BIN}/bin/xdrc)

if(NOT APPLE)
    if(DEFINED ENV{CC} AND DEFINED ENV{CXX})
        set(XDRPP_CONFIGURE_COMMAND ${XDRPP_DIR}/configure --srcdir=${XDRPP_DIR} --prefix=${XDRPP_BIN} --enable-static=yes --disable-shared CC=$ENV{CC} CXX=$ENV{CXX})
    endif()
endif()

if(NOT DEFINED XDRPP_CONFIGURE_COMMAND)
    set(XDRPP_CONFIGURE_COMMAND ${XDRPP_DIR}/configure --srcdir=${XDRPP_DIR} --prefix=${XDRPP_BIN} --enable-static=yes --disable-shared)
endif()

ExternalProject_Add(libxdrpp
    PREFIX ${XDRPP_BIN}
    SOURCE_DIR ${XDRPP_DIR}
    DOWNLOAD_COMMAND cd ${XDRPP_DIR} && ${XDRPP_DIR}/autogen.sh
    CONFIGURE_COMMAND ${XDRPP_CONFIGURE_COMMAND}
    BUILD_COMMAND ${MAKE}
    INSTALL_COMMAND make install
    BUILD_BYPRODUCTS ${XDRPP_STATIC_LIB})
    file(MAKE_DIRECTORY ${XDRPP_INCLUDES})

    add_library(xdrpp STATIC IMPORTED GLOBAL)

    add_dependencies(xdrpp libxdrpp)

    set_target_properties(xdrpp PROPERTIES IMPORTED_LOCATION ${XDRPP_STATIC_LIB})
    set_target_properties(xdrpp PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${XDRPP_INCLUDES})

    add_executable(xdrc IMPORTED GLOBAL)
    add_dependencies(xdrc libxdrpp)
    set_target_properties(xdrpp PROPERTIES IMPORTED_LOCATION ${XDRC_EXECUTABLE})
endif()
