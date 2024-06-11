# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp test2.cpp' are source files with tests
add_executable(Line_UT
        src/LineUnitTests.cpp
        ../src/instance/graph/Node.cpp
        ../src/instance/graph/Node.h
        ../src/instance/graph/Edge.cpp
        ../src/instance/graph/Edge.h
        ../src/instance/graph/Graph.cpp
        ../src/instance/graph/Graph.h
        ../src/instance/graph/Line.cpp
        ../src/instance/graph/Line.h
)

add_executable(Transit_preprocess_DEB
        src/TransitPreprocessDebug.cpp
        ../src/instance/graph/Node.cpp
        ../src/instance/graph/Node.h
        ../src/instance/graph/Edge.cpp
        ../src/instance/graph/Edge.h
        ../src/instance/graph/Graph.cpp
        ../src/instance/graph/Graph.h
        ../src/instance/graph/Line.cpp
        ../src/instance/graph/Line.h
        ../src/routes/KeyPoint.cpp
        ../src/routes/KeyPoint.h
        ../src/ShortestPath/Transit/TransitShortestPathContainer.cpp
        ../src/ShortestPath/Transit/TransitShortestPathContainer.h
        ../src/ShortestPath/Transit/TransitShortestPathPrecompute.cpp
        ../src/ShortestPath/Transit/TransitShortestPathPrecompute.h
)


add_executable(Constraint_Propag_DEB
        src/ConstraintPropagationDebug.cpp
        ../src/TimeWindow.cpp
        ../src/TimeWindow.h
        ../src/instance/graph/Node.cpp
        ../src/instance/graph/Node.h
        ../src/instance/graph/Edge.cpp
        ../src/instance/graph/Edge.h
        ../src/instance/graph/Graph.cpp
        ../src/instance/graph/Graph.h
        ../src/instance/graph/Line.cpp
        ../src/instance/graph/Line.h
        ../src/instance/requests/Request.cpp
        ../src/instance/requests/Request.h
        ../src/routes/vehicle/SAEVRoute.cpp
        ../src/routes/vehicle/SAEVRoute.h
        ../src/routes/vehicle/propagation/SAEVRouteChange.cpp
        ../src/routes/vehicle/propagation/SAEVRouteChange.h
        ../src/routes/vehicle/propagation/SAEVRouteChangelist.cpp
        ../src/routes/vehicle/propagation/SAEVRouteChangelist.h
        ../src/routes/vehicle/SAEVKeyPoint.cpp
        ../src/routes/vehicle/SAEVKeyPoint.h
        ../src/routes/KeyPoint.cpp
        ../src/routes/KeyPoint.h
        ../src/routes/RequestKeyPoint.cpp
        ../src/routes/RequestKeyPoint.h
        ../src/routes/RequestRoute.cpp
        ../src/routes/RequestRoute.h
        ../src/utils/InstanceGenerator.cpp
        ../src/utils/InstanceGenerator.h
)

target_link_libraries(Line_UT gtest gtest_main)
target_link_libraries(Transit_preprocess_DEB gtest gtest_main)
target_link_libraries(Constraint_Propag_DEB gtest gtest_main)