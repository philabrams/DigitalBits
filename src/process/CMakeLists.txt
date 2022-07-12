target_sources(core
    PRIVATE
        PosixSpawnFileActions.cpp
        PosixSpawnFileActions.h
        ProcessManager.h
        ProcessManagerImpl.cpp
        ProcessManagerImpl.h

        $<$<BOOL:${BUILD_TESTS_OPTION}>:${CMAKE_CURRENT_SOURCE_DIR}/test/ProcessTests.cpp>
)