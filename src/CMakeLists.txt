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
set(CORE_EXE_OUTPUT_NAME digitalbits-core)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/main/DigitalBitsCoreVersion.cpp.in
               ${CMAKE_CURRENT_SOURCE_DIR}/main/DigitalBitsCoreVersion.cpp)

add_executable(core "")
set_target_properties(core PROPERTIES OUTPUT_NAME ${CORE_EXE_OUTPUT_NAME})

if(MSVC)
  target_compile_definitions(core PRIVATE /bigobj)
else()
  target_compile_options(core PUBLIC 
    "$<$<CONFIG:DEBUG>:-g>"
    "$<$<CONFIG:DEBUG>:-O0>"
    "$<$<CONFIG:RELEASE>:-O3>"
  )
endif()

set(XDR_CORE_HEADER_NAMES DigitalBits-SCP DigitalBits-ledger-entries DigitalBits-ledger DigitalBits-overlay DigitalBits-transaction DigitalBits-types)
set(XDR_CORE_HEADERS)

foreach(_file ${XDR_CORE_HEADER_NAMES})
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/xdr/${_file}.h
    COMMAND xdrc -hh -o ${CMAKE_CURRENT_SOURCE_DIR}/xdr/${_file}.h ${CMAKE_CURRENT_SOURCE_DIR}/xdr/${_file}.x
    DEPENDS xdrc
    VERBATIM)
  list(APPEND XDR_CORE_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/xdr/${_file}.h)
endforeach()

# Copy testdata
file(COPY 
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.testnet.6714239.json
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.livenet.15686975.json
  ${CMAKE_SOURCE_DIR}/src/history/serialize-tests/digitalbits-history.testnet.6714239.networkPassphrase.json
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_example.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_standalone.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_testnet.cfg 
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_testnet_legacy.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_testnet_validator.cfg
  ${CMAKE_SOURCE_DIR}/docs/digitalbits-core_example_validators.cfg
  DESTINATION ${CMAKE_SOURCE_DIR}/src/testdata
)


target_sources(core PRIVATE ${XDR_CORE_HEADERS})

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
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/lib/spdlog/include>
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
    $<$<CXX_COMPILER_ID:MSVC>:credui>
)

install(TARGETS core)

if(UNIX AND BUILD_TESTS_OPTION)
  if(USE_POSTGRES_OPTION)
    set(DB_SELF_TEST_CMD ./test/selftest-pg)
  else()
    set(DB_SELF_TEST_CMD ./test/selftest-nopg)
  endif()

  add_custom_target(check
    COMMAND cp ${CMAKE_BINARY_DIR}/src/${CORE_EXE_OUTPUT_NAME} ${CMAKE_SOURCE_DIR}/src && ${DB_SELF_TEST_CMD}
    COMMENT "Running unit tests..."
    DEPENDS core
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
  )
endif()
