cmake_minimum_required(VERSION 3.27)
project(GreedyAlgorithm)

set(CMAKE_CXX_STANDARD 23)

add_definitions(-DDEBUG_TRANSIT_PRECOMPUTE)

add_subdirectory(test)

#set(PYTHON_INCLUDE_DIRS "/usr/include/python3.10/")
find_package (Python COMPONENTS Interpreter Development)

add_executable(GreedyAlgorithm
        src/instance/graph/Node.cpp
        src/instance/graph/Node.h
        src/instance/graph/Edge.cpp
        src/instance/graph/Edge.h
        src/instance/graph/Graph.cpp
        src/instance/graph/Graph.h
        src/instance/requests/Request.cpp
        src/instance/requests/Request.h
        src/instance/Instance.cpp
        src/instance/Instance.h
        src/instance/graph/Line.cpp
        src/instance/graph/Line.h
        src/services/CSV/CSVRange.h
        src/services/CSV/CSVIterator.h
        test/debug.cpp
        src/instance/graph/LineStop.cpp
        src/instance/graph/LineStop.h
        src/services/DatFile/DATRow.h
        src/utils/SearchAlgorithms.h
        src/ShortestPath/ShortestPathContainer.h
        src/ShortestPath/TimeDependentShortestPathContainer.h
        src/ShortestPath/TimeDependentShortestPathContainer.h
        src/ShortestPath/ShortestPath.h
        src/ShortestPath/Transit/TransitShortestPath.h
        src/ShortestPath/Transit/TransitShortestPathPrecompute.cpp
        src/ShortestPath/Transit/TransitShortestPathPrecompute.h
        src/ShortestPath/Transit/TransitAlgorithmState.cpp
        src/ShortestPath/Transit/TransitAlgorithmState.h
        src/ShortestPath/Transit/TransitShortestPathContainer.cpp
        src/ShortestPath/Transit/TransitShortestPathContainer.h
        src/routes/RequestRoute.cpp
        src/routes/RequestRoute.h
        src/routes/SAEVRoute.cpp
        src/routes/SAEVRoute.h
        src/TimeWindow.cpp
        src/TimeWindow.h
        src/routes/KeyPoint.cpp
        src/routes/KeyPoint.h
        src/routes/RequestKeyPoint.cpp
        src/routes/RequestKeyPoint.h
        src/routes/SAEVKeyPoint.cpp
        src/routes/SAEVKeyPoint.h
        src/instance/SAEVehicle.cpp
        src/instance/SAEVehicle.h
)

target_include_directories(GreedyAlgorithm PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(GreedyAlgorithm ${PYTHON_LIBRARIES})

