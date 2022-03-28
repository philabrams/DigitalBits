message("Building src")

find_package(AWSSDK REQUIRED COMPONENTS secretsmanager)
find_package(ZLIB)

# Pull the package version from git tags
execute_process(COMMAND git describe --always --tags --dirty
                OUTPUT_VARIABLE VERSION
                ERROR_QUIET)

string(REGEX REPLACE "^([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.[0-9]+(.*)" "\\1" VERSION_SHA1 "${VERSION}")

set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main/DigitalBitsCoreVersion.cpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/main/DigitalBitsCoreVersion.cpp)

add_executable(core "")
set_target_properties(core PROPERTIES OUTPUT_NAME "digitalbits-core")

# Convert XDR files to headers
add_custom_command(
  OUTPUT xdr/DigitalBits-SCP.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-SCP.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-SCP.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-SCP.x
  VERBATIM)

add_custom_command(
  OUTPUT xdr/DigitalBits-ledger-entries.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger-entries.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger-entries.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger-entries.x
  VERBATIM)

add_custom_command(
  OUTPUT xdr/DigitalBits-ledger.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-ledger.x
  VERBATIM)

add_custom_command(
  OUTPUT xdr/DigitalBits-overlay.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-overlay.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-overlay.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-overlay.x
  VERBATIM)

add_custom_command(
  OUTPUT xdr/DigitalBits-transaction.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-transaction.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-transaction.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-transaction.x
  VERBATIM)

add_custom_command(
  OUTPUT xdr/DigitalBits-types.h
  COMMAND ${CMAKE_BINARY_DIR}/lib/xdrpp/xdrpp/bin/xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-types.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-types.x
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/DigitalBits-types.x
  VERBATIM)

# Copy testdata
file(COPY 
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.testnet.6714239.json
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.livenet.15686975.json
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.testnet.6714239.networkPassphrase.json
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_example.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_standalone.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_testnet.cfg 
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_testnet_legacy.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_example_validators.cfg
  DESTINATION ${CMAKE_SOURCE_DIR}/src/testdata
)


target_sources(core
  PRIVATE
    xdr/DigitalBits-SCP.h
    xdr/DigitalBits-ledger-entries.h
    xdr/DigitalBits-ledger.h
    xdr/DigitalBits-overlay.h
    xdr/DigitalBits-transaction.h
    xdr/DigitalBits-types.h
)

# Add sources
add_subdirectory(bucket)
add_subdirectory(catchup)
add_subdirectory(crypto)
add_subdirectory(database)
add_subdirectory(herder)
add_subdirectory(history)
add_subdirectory(historywork)
add_subdirectory(invariant)
add_subdirectory(ledger)
add_subdirectory(main)
add_subdirectory(overlay)
add_subdirectory(process)
add_subdirectory(scp)
add_subdirectory(secrets)
add_subdirectory(transactions)
add_subdirectory(util)
add_subdirectory(work)

if(BUILD_TESTS_OPTION)
  add_subdirectory(simulation)
  add_subdirectory(test)
endif()

target_include_directories(core PRIVATE
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/asio/asio/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/autocheck/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/cereal/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/fmt/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/json>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/libmedida/src>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/libsodium/src/libsodium/include>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/soci/src/core>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/tracy>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/util>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/sqlite>
)

if(HAVE_LIBUNWIND AND NOT APPLE)
  set(LINK_LIBUNWIND ON)
else()
  set(LINK_LIBUNWIND OFF)
endif()

target_link_libraries(core
  PRIVATE
    fmt
    medida
    xdrpp
    soci
    sodium
    lib3rdparty
    $<$<BOOL:${USE_SPDLOG_OPTION}>:spdlog>
    ${AWSSDK_LINK_LIBRARIES}
    $<$<BOOL:${LINK_LIBUNWIND}>:unwind>
)
