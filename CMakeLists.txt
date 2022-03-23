cmake_minimum_required(VERSION 3.22)

include(CheckIncludeFiles)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")
set(BUILD_SHARED_LIBS OFF CACHE STRING "Do not link to shared libraries by default.")

project(digitalbits-core LANGUAGES C CXX)

# Configuration options
# Use either easylogging or spdlog
option(USE_EASYLOGGING_OPTION "Use easylogging library" ON)
option(USE_SPDLOG_OPTION "Use spdlog library" OFF)

option(BUILD_TESTS_OPTION "Build unit test files" ON)
option(USE_POSTGRES_OPTION "Use PostgreSQL for storage" ON)

# Additional platform checks
CHECK_INCLUDE_FILES(libunwind.h HAVE_LIBUNWIND)

# Define preprocessor macros
if(USE_EASYLOGGING_OPTION AND (NOT USE_SPDLOG_OPTION))
    message("Using Easylogging++ for logging.")
    add_compile_definitions(USE_EASYLOGGING)
endif()

if(USE_SPDLOG_OPTION AND (NOT USE_EASYLOGGING_OPTION))
    message("Using spdlog for logging.")
    add_compile_definitions(USE_SPDLOG)
endif()

# We should configure option inclusion of unit tests
if(BUILD_TESTS_OPTION)
    message("Unit test files are included.")
    add_compile_definitions(BUILD_TESTS=1)
endif()

if(USE_POSTGRES_OPTION)
  find_package(PostgreSQL REQUIRED)
  add_compile_definitions(USE_POSTGRES=1)
endif()

add_compile_definitions(ASIO_SEPARATE_COMPILATION=1)
add_compile_definitions(ASIO_STANDALONE)
add_compile_definitions(SQLITE_CORE)
add_compile_definitions(SQLITE_OMIT_LOAD_EXTENSION=1)

add_compile_options(
  "$<$<COMPILE_LANG_AND_ID:C,CXX,AppleClang,Clang,GNU>:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic>>"
  "$<$<COMPILE_LANG_AND_ID:C,CXX,MSVC>:$<BUILD_INTERFACE:-W4>>"
)

message(${CMAKE_SOURCE_DIR})

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_SOURCE_DIR}/config.h)

add_subdirectory(lib)
add_subdirectory(src)
