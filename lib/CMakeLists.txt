include(ExternalProject)

add_library(lib3rdparty STATIC "")
set_target_properties(lib3rdparty PROPERTIES OUTPUT_NAME "lib3rdparty")

# Cmake projects
add_subdirectory(fmt)

if(MSVC)
    message(${CMAKE_BINARY_DIR})
    set(MEDIDA_STATIC_LIB_DEBUG ${CMAKE_BINARY_DIR}/x64/Debug/libmedida.lib)
    set(MEDIDA_STATIC_LIB_RELEASE ${CMAKE_BINARY_DIR}/x64/Release/libmedida.lib)

    include_external_msproject(libmedida ${CMAKE_CURRENT_SOURCE_DIR}/libmedida/libmedida.vcxproj)

    add_library(medida STATIC IMPORTED GLOBAL)
    add_dependencies(medida libmedida)

    set_target_properties(medida PROPERTIES
        IMPORTED_LOCATION "${MEDIDA_STATIC_LIB_RELEASE}"
        IMPORTED_LOCATION_DEBUG "${MEDIDA_STATIC_LIB_DEBUG}"
        IMPORTED_CONFIGURATIONS "RELEASE;DEBUG")
else()
    add_subdirectory(libmedida)
endif()

add_subdirectory(soci)

if(USE_SPDLOG_OPTION)
    add_subdirectory(spdlog)
endif()

# autools projects
add_subdirectory(libsodium)
add_subdirectory(xdrpp)

# lib3rdparty sources
add_subdirectory(http)
add_subdirectory(json)
add_subdirectory(sqlite)
add_subdirectory(tracy)
add_subdirectory(util)

target_sources(lib3rdparty
    PRIVATE
        asio.cpp
)

target_include_directories(lib3rdparty PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/asio/asio/include>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/fmt/include>
)
