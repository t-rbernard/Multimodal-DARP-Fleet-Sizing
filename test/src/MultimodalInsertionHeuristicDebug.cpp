//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"
#include "../../src/utils/Timing.h"
#include "../../src/algorithm/Multimodal/Heuristics/SimpleModularHeuristic.h"
#include "../../src/utils/Instance Generation/Requests/RequestsGenerator.h"

TEST(MultimodalInsertionHeuristicDebug, DebugBaseInstance) {
    std::string instancesPath = "../../resources/test/instances/PT Shortest Path/";
    std::string instanceFolder = "contiguous_lines_debug_instance/";
    std::string datFile = "graph.dat";


    //Parse graph
    INIT_TIMER
    Graph graphFromSingleFile(instancesPath + instanceFolder + datFile);
    graphFromSingleFile.computeAndUpdateShortestPathsMatrix(true);
    RequestGenerationParameters genParams(1, 1.5, 15,30,300,600,290820241032L);
    std::vector<Request> requests = RequestsGenerator::generateRequests(graphFromSingleFile, genParams);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests, true);
    SimpleModularHeuristic multimodalHeuristic(&graphFromSingleFile, &routesContainer, &requests);
    STOP_TIMER("Instance parsing and init")
    std::cout << "------------------Fin parsing instance et route-------------------" << std::endl << std::endl;

    std::cout << "------------------Start preprocessings-------------------" << std::endl << std::endl;
    START_TIMER
    graphFromSingleFile.computeAndUpdateShortestTransitPaths();
    STOP_TIMER("Preprocess")
    std::cout << "------------------End preprocessings-------------------" << std::endl << std::endl;
    std::cout << "------------------Start multimodal insertion (entry)-------------------" << std::endl << std::endl;
    START_TIMER
    for(size_t i = 0; i < multimodalHeuristic.getNbBaseRquests(); ++i) {
        multimodalHeuristic.insertBestTransitEntryInRoute(requests[i], i);
    }
    STOP_TIMER("Multimodal insertion (entry)")
    std::cout << "------------------End multimodal insertion (entry)-------------------" << std::endl << std::endl;
    std::cout << "------------------Start multimodal insertion (exit)-------------------" << std::endl << std::endl;
    START_TIMER
    for(size_t i = 0; i < multimodalHeuristic.getNbBaseRquests(); ++i) {
        multimodalHeuristic.insertBestTransitExitsInRoute(requests[i], i);
    }
    STOP_TIMER("Multimodal insertion (exit)")
    std::cout << "------------------End multimodal insertion (exit)-------------------" << std::endl << std::endl;
}

TEST(MultimodalInsertionHeuristicDebug, DebugInstanceAlain) {
    std::string instancesPath = "../../resources/test/instances/MultimodalHeuristic/";
    std::string instanceFolder = "instance_alain_140624/";
    std::string graphDatFile = "graph.dat";
    std::string requestDatFile = "even_more_requests.dat";


    //Parse graph
    INIT_TIMER
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    graphFromSingleFile.computeAndUpdateShortestPathsMatrix(true);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests, true);
    SimpleModularHeuristic multimodalHeuristic(&graphFromSingleFile, &routesContainer, &requests);
    STOP_TIMER("Instance parsing and init")
    std::cout << "------------------Fin parsing instance et route-------------------" << std::endl << std::endl;

    std::cout << "------------------Start preprocessings-------------------" << std::endl << std::endl;
    START_TIMER
    graphFromSingleFile.computeAndUpdateShortestTransitPaths();
    STOP_TIMER("Preprocess")
    std::cout << "------------------End preprocessings-------------------" << std::endl << std::endl;
    std::cout << "------------------Start multimodal insertion (entry)-------------------" << std::endl << std::endl;
    START_TIMER
    for(size_t i = 0; i < multimodalHeuristic.getNbBaseRquests(); ++i) {
        multimodalHeuristic.insertBestTransitEntryInRoute(requests[i], i);
    }
    STOP_TIMER("Multimodal insertion (entry)")
    std::cout << "------------------End multimodal insertion (entry)-------------------" << std::endl << std::endl;
    std::cout << "------------------Start multimodal insertion (exit)-------------------" << std::endl << std::endl;
    START_TIMER
    for(size_t i = 0; i < multimodalHeuristic.getNbBaseRquests(); ++i) {
        multimodalHeuristic.insertBestTransitExitsInRoute(requests[i], i);
    }
    STOP_TIMER("Multimodal insertion (exit)")
    std::cout << "------------------End multimodal insertion (exit)-------------------" << std::endl << std::endl;
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}