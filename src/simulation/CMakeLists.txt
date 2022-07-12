target_sources(core
    PRIVATE
        CoreTests.cpp
        LoadGenerator.cpp
        LoadGenerator.h
        Simulation.cpp
        Simulation.h
        Topologies.cpp
        Topologies.h

        $<$<BOOL:${BUILD_TESTS_OPTION}>:${CMAKE_CURRENT_SOURCE_DIR}/test/LoadGeneratorTests.cpp>
)
