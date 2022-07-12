target_sources(core
    PRIVATE
        Database.cpp
        Database.h
        DatabaseConnectionString.cpp
        DatabaseConnectionString.h
        DatabaseTypeSpecificOperation.h
        DatabaseUtils.cpp
        DatabaseUtils.h

        $<$<BOOL:${BUILD_TESTS_OPTION}>:${CMAKE_CURRENT_SOURCE_DIR}/test/DatabaseConnectionStringTest.cpp>
        $<$<BOOL:${BUILD_TESTS_OPTION}>:${CMAKE_CURRENT_SOURCE_DIR}/test/DatabaseTests.cpp>
)