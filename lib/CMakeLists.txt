message("Bulding libs")

include(ExternalProject)

add_library(lib3rdparty STATIC "")
set_target_properties(lib3rdparty PROPERTIES OUTPUT_NAME "lib3rdparty")

# Cmake projects
add_subdirectory(fmt)
add_subdirectory(libmedida)
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
