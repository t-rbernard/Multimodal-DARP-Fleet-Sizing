cmake_minimum_required(VERSION 3.28.1)
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
        src/instance/graph/LineStop.cpp
        src/instance/graph/LineStop.h
        src/services/DatFile/DATRow.h
        src/utils/SearchAlgorithms.h
        src/ShortestPath/ShortestPath.h
        src/ShortestPath/Transit/TransitShortestPath.h
        src/ShortestPath/Transit/TransitShortestPathPrecompute.cpp
        src/ShortestPath/Transit/TransitShortestPathPrecompute.h
        src/ShortestPath/Transit/TransitAlgorithmState.cpp
        src/ShortestPath/Transit/TransitAlgorithmState.h
        src/ShortestPath/Transit/TransitShortestPathContainer.cpp
        src/ShortestPath/Transit/TransitShortestPathContainer.h
        src/routes/requests/RequestRoute.cpp
        src/routes/requests/RequestRoute.h
        src/routes/vehicle/SAEVRoute.cpp
        src/routes/vehicle/SAEVRoute.h
        src/TimeWindow.cpp
        src/TimeWindow.h
        src/routes/KeyPoint.cpp
        src/routes/KeyPoint.h
        src/routes/requests/RequestKeyPoint.cpp
        src/routes/requests/RequestKeyPoint.h
        src/routes/vehicle/SAEVKeyPoint.cpp
        src/routes/vehicle/SAEVKeyPoint.h
        src/instance/SAEVehicle.cpp
        src/instance/SAEVehicle.h
        src/routes/vehicle/propagation/SAEVRouteChangelist.cpp
        src/routes/vehicle/propagation/SAEVRouteChangelist.h
        src/routes/vehicle/propagation/SAEVRouteChange.cpp
        src/routes/vehicle/propagation/SAEVRouteChange.h
        "src/utils/Instance Generation/Requests/RequestsGenerator.cpp"
        "src/utils/Instance Generation/Requests/RequestsGenerator.h"
        "src/utils/Instance Generation/Requests/RequestGenerationParameters.h"
        "src/utils/Instance Generation/Graph/OSRMGraphGenerator.h"
        src/routes/vehicle/BestRequestInsertion.h
        src/routes/vehicle/BestInsertionQueue.h
        src/utils/Constants.h
)

add_executable(RequestGenerator
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
        src/instance/graph/LineStop.cpp
        src/instance/graph/LineStop.h
        src/services/DatFile/DATRow.h
        src/routes/vehicle/SAEVRoute.cpp
        src/routes/vehicle/SAEVRoute.h
        src/TimeWindow.cpp
        src/TimeWindow.h
        "src/utils/Instance Generation/Requests/RequestsGenerator.cpp"
        "src/utils/Instance Generation/Requests/RequestsGenerator.h"
        "src/utils/Instance Generation/Requests/RequestGenerationParameters.h"
        "src/utils/Instance Generation/Graph/OSRMGraphGenerator.h"
)

add_executable(GraphGeneration
        src/instance/graph/Node.cpp
        src/instance/graph/Node.h
        src/instance/graph/Edge.cpp
        src/instance/graph/Edge.h
        src/instance/graph/Graph.cpp
        src/instance/graph/Graph.h
        src/instance/Instance.cpp
        src/instance/Instance.h
        src/instance/graph/Line.cpp
        src/instance/graph/Line.h
        src/services/CSV/CSVRange.h
        src/services/CSV/CSVIterator.h
        src/instance/graph/LineStop.cpp
        src/instance/graph/LineStop.h
        src/services/DatFile/DATRow.h
        "src/utils/Instance Generation/Graph/OSRMGraphGenerator.h"
        "src/utils/Instance Generation/Graph/GraphGenerator.h"
        "src/utils/Instance Generation/Graph/PTLineGenerationParameters.h"
        src/algorithm/DARP/Heuristics/BestInsertionHeuristic.cpp
        src/algorithm/DARP/Heuristics/BestInsertionHeuristic.h
        src/algorithm/Multimodal/Heuristics/SimpleModularHeuristic.cpp
        src/algorithm/Multimodal/Heuristics/SimpleModularHeuristic.h
        src/ShortestPath/Vehicle/VehicleShortestPath.cpp
        src/ShortestPath/Vehicle/VehicleShortestPath.h
        src/ShortestPath/Vehicle/VehiclePathState.h
        src/ShortestPath/Vehicle/VehicleShortestPathCalculation.cpp
        src/ShortestPath/Vehicle/VehicleShortestPathCalculation.h
        src/ShortestPath/Vehicle/VehicleShortestPathContainer.cpp
        src/ShortestPath/Vehicle/VehicleShortestPathContainer.h
)

target_include_directories(GreedyAlgorithm PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(GreedyAlgorithm ${PYTHON_LIBRARIES})

