//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"
#include "../../src/utils/Timing.h"
#include "../../src/algorithm/Multimodal/Heuristics/SimpleModularHeuristic.h"

TEST(MultimodalInsertionHeuristicDebug, DebugBaseInstance) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "basic_debug_instance/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    INIT_TIMER
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);
    SimpleModularHeuristic multimodalHeuristic(&graphFromSingleFile, &routesContainer, &requests);
    STOP_TIMER("Parsing and init")
    std::cout << "------------------Fin parsing instance et route-------------------" << std::endl << std::endl;

    std::cout << "------------------Start preprocessings-------------------" << std::endl << std::endl;
    START_TIMER
    routesContainer.initMultimodalKeyPoints();
    graphFromSingleFile.computeAndUpdateShortestPathsMatrix();
    graphFromSingleFile.computeAndUpdateShortestTransitPaths();
    STOP_TIMER("Preprocess")
    std::cout << "------------------End preprocessings-------------------" << std::endl << std::endl;
    for(size_t i = 0; i < requests.size(); ++i) {
        multimodalHeuristic.insertBestTransitEntryInRoute(requests[i], i);
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}