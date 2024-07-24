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
        ../src/utils/Globals.h
)

add_executable(Transit_Preprocess_DEB
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
        ../src/utils/Globals.h
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
        ../src/routes/requests/RequestKeyPoint.cpp
        ../src/routes/requests/RequestKeyPoint.h
        ../src/routes/requests/RequestRoute.cpp
        ../src/routes/requests/RequestRoute.h
        "../src/utils/Instance Generation/Requests/RequestsGenerator.cpp"
        "../src/utils/Instance Generation/Requests/RequestsGenerator.h"
        "../src/utils/Instance Generation/Requests/RequestGenerationParameters.h"
        "../src/utils/Instance Generation/Graph/OSRMGraphGenerator.h"
        ../src/utils/Constants.h
        ../src/ShortestPath/Vehicle/VehiclePathState.h
        ../src/ShortestPath/Vehicle/VehicleShortestPath.h
        ../src/ShortestPath/Vehicle/VehicleShortestPathCalculation.h
        ../src/ShortestPath/Vehicle/VehicleShortestPathCalculation.cpp
        ../src/ShortestPath/Vehicle/MatrixShortestPathContainer.h
)

add_executable(BestInsertionHeuristic_DEB
        src/BestInsertionHeuristicDebug.cpp
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
        ../src/routes/requests/RequestKeyPoint.cpp
        ../src/routes/requests/RequestKeyPoint.h
        ../src/routes/requests/RequestRoute.cpp
        ../src/routes/requests/RequestRoute.h
        ../src/routes/vehicle/BestRequestInsertion.h
        ../src/routes/vehicle/BestInsertionQueue.h
        ../src/algorithm/DARP/Heuristics/BestInsertionHeuristic.cpp
        ../src/algorithm/DARP/Heuristics/BestInsertionHeuristic.h
        ../src/utils/Constants.h
)

add_executable(GraphPreprocess_DEB
        src/GraphPreprocessDebug.cpp
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
        ../src/routes/requests/RequestKeyPoint.cpp
        ../src/routes/requests/RequestKeyPoint.h
        ../src/routes/requests/RequestRoute.cpp
        ../src/routes/requests/RequestRoute.h
        ../src/ShortestPath/Vehicle/VehicleShortestPathCalculation.h
        ../src/ShortestPath/Vehicle/VehicleShortestPathCalculation.cpp
        ../src/utils/Constants.h
)

target_link_libraries(Line_UT gtest gtest_main)
target_link_libraries(Transit_Preprocess_DEB gtest gtest_main)
target_link_libraries(Constraint_Propag_DEB gtest gtest_main)
target_link_libraries(BestInsertionHeuristic_DEB gtest gtest_main)
target_link_libraries(GraphPreprocess_DEB gtest gtest_main)