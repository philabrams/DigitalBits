cmake_minimum_required(VERSION 3.22)

include(CheckIncludeFiles)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(digitalbits-core LANGUAGES C CXX)
set(BUILD_SHARED_LIBS OFF CACHE STRING "Do not link to shared libraries by default.")

if(MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

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
    message(STATUS "Using Easylogging++ for logging.")
    add_compile_definitions(USE_EASYLOGGING)
endif()

if(USE_SPDLOG_OPTION AND (NOT USE_EASYLOGGING_OPTION))
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
add_compile_definitions(TRACY_ENABLE)
add_compile_definitions(TRACY_ON_DEMAND)
add_compile_definitions(TRACY_NO_BROADCAST)
add_compile_definitions(TRACY_ONLY_LOCALHOST)
add_compile_definitions(USE_TRACY)

if(MSVC)
  add_compile_definitions(WIN32_LEAN_AND_MEAN)
  add_compile_definitions(NOMINMAX)
  add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  add_compile_definitions(_WIN32_WINNT=0x0601)
  add_compile_definitions(_MBCS)
  add_compile_definitions(_CRT_NONSTDC_NO_DEPRECATE)
endif()

add_compile_options(
  "$<$<COMPILE_LANG_AND_ID:C,CXX,AppleClang,Clang,GNU>:$<BUILD_INTERFACE:-Wall;-Wextra;-Wpedantic>>"
  "$<$<COMPILE_LANG_AND_ID:C,CXX,MSVC>:$<BUILD_INTERFACE:-W4>>"
)

CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/config.h.in ${CMAKE_SOURCE_DIR}/config.h)

add_subdirectory(lib)
add_subdirectory(src)
