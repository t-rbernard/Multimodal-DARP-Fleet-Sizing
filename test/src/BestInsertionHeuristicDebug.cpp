//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"

TEST(BestInsertionHeuristicDebug, DebugBaseInstance) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "basic_debug_instance/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    std::cout << "------------------Fin parsing instance et route-------------------" << std::endl << std::endl;
    int vehicleId = 1;
    assert(routesContainer.checkRouteTimeWindows(vehicleId));
    SAEVRouteChangelist req0Changelist = routesContainer.tryAddRequest(0, routesContainer.getOriginDepot(vehicleId), routesContainer.getOriginDepot(1));
    std::cout << routesContainer.to_string(vehicleId) << std::endl;
    assert(routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;
    SAEVRouteChangelist req1Changelist = routesContainer.tryAddRequest(1, routesContainer.getOriginDepot(vehicleId), routesContainer.getDestination(0));
    std::cout << routesContainer.to_string(vehicleId) << std::endl << std::endl;
    assert(!routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;

    //Test changelist revert/apply
    req1Changelist.revertChanges();
    req1Changelist.applyChanges();
    req1Changelist.revertChanges();
    req0Changelist.revertChanges();
}

TEST(BestInsertionHeuristicDebug, DebugInstanceAlain) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "Instance_Alain_140624/";
    std::string graphDatFile = "graph2.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    //Vehicle 1 insertions
    BestInsertionQueue biQueue = routesContainer.getBestInsertionsQueue(0,0);
    routesContainer.tryAddRequest(0,routesContainer.getOriginDepot(0),routesContainer.getOriginDepot(0));
    biQueue = routesContainer.getBestInsertionsQueue(1,0);
    routesContainer.tryAddRequest(1,routesContainer.getOrigin(0),routesContainer.getOrigin(0));
    biQueue = routesContainer.getBestInsertionsQueue(2,0);
    SAEVRouteChangelist cl = routesContainer.tryAddRequest(2,routesContainer.getOrigin(1),routesContainer.getDestination(1));
    biQueue = routesContainer.getBestInsertionsQueue(3,0);

    //Vehicle 2 insertions
    biQueue = routesContainer.getBestInsertionsQueue(5,1);
    routesContainer.tryAddRequest(5,routesContainer.getOriginDepot(1),routesContainer.getOriginDepot(1));
    biQueue = routesContainer.getBestInsertionsQueue(4,1);
    routesContainer.tryAddRequest(4,routesContainer.getOriginDepot(1),routesContainer.getDestination(5));
    biQueue = routesContainer.getBestInsertionsQueue(3,1);
    routesContainer.tryAddRequest(3,routesContainer.getOriginDepot(1),routesContainer.getOrigin(4));
    biQueue = routesContainer.getBestInsertionsQueue(0,1);
    biQueue = routesContainer.getBestInsertionsQueue(1,1);
    biQueue = routesContainer.getBestInsertionsQueue(2,1);
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}