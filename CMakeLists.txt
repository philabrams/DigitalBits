cmake_minimum_required(VERSION 3.22)

include(CheckIncludeFiles)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

project(digitalbits-core LANGUAGES C CXX)
set(BUILD_SHARED_LIBS OFF CACHE STRING "Do not link to shared libraries by default.")

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# Configuration options
option(USE_SPDLOG_OPTION "Use spdlog library" ON)
option(BUILD_TESTS_OPTION "Build unit test files" ON)
option(USE_POSTGRES_OPTION "Use PostgreSQL for storage" ON)
option(USE_TRACY_OPTION "Enable profiling" OFF)
option(ENABLE_NEXT_PROTOCOL_VERSION_UNSAFE_FOR_PRODUCTION "Enable next protocol version, UNSAFE FOR PRODUCTION" OFF)

# Additional platform checks
# CHECK_INCLUDE_FILES(libunwind.h HAVE_LIBUNWIND)

# Define preprocessor macros
if(USE_SPDLOG_OPTION)
    message(STATUS "Using spdlog for logging.")
    add_compile_definitions(USE_SPDLOG)
endif()

# We should configure option inclusion of unit tests
if(BUILD_TESTS_OPTION)
    message(STATUS "Unit test files are included.")
    add_compile_definitions(BUILD_TESTS=1)
endif()

if(USE_POSTGRES_OPTION)
  find_package(PostgreSQL REQUIRED)
  add_compile_definitions(USE_POSTGRES=1)
endif()

add_compile_definitions(ASIO_SEPARATE_COMPILATION=1)
add_compile_definitions(ASIO_STANDALONE)
add_compile_definitions(ASIO_ERROR_CATEGORY_NOEXCEPT=noexcept)
add_compile_definitions(SQLITE_CORE)
add_compile_definitions(SQLITE_OMIT_LOAD_EXTENSION=1)
add_compile_definitions(SODIUM_STATIC)

if(USE_TRACY_OPTION)
  add_compile_definitions(USE_TRACY)
  add_compile_definitions(TRACY_ENABLE)
  add_compile_definitions(TRACY_ON_DEMAND)
  add_compile_definitions(TRACY_NO_BROADCAST)
  add_compile_definitions(TRACY_ONLY_LOCALHOST)
  add_compile_definitions(TRACY_ONLY_IPV4)
endif()

if(ENABLE_NEXT_PROTOCOL_VERSION_UNSAFE_FOR_PRODUCTION)
  message(WARNING "Enabled next protocol version, UNSAFE FOR PRODUCTION")
  add_compile_definitions(ENABLE_NEXT_PROTOCOL_VERSION_UNSAFE_FOR_PRODUCTION)
endif()

if(MSVC)
  add_compile_definitions(WIN32_LEAN_AND_MEAN)
  add_compile_definitions(NOMINMAX)
  add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  add_compile_definitions(_WIN32_WINNT=0x0601)
  add_compile_definitions(_MBCS)
  add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_SOURCE_DIR}/config.h)

add_subdirectory(lib)
add_subdirectory(src)
