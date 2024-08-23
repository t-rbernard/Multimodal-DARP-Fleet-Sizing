//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"
#include "../../src/algorithm/DARP/Heuristics/BestInsertionHeuristic.h"

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
    SAEVRouteChangelist req0Changelist = routesContainer.tryAddRequest(0, &routesContainer.getOriginDepot(vehicleId), &routesContainer.getOriginDepot(1));
    std::cout << routesContainer.to_string(vehicleId) << std::endl;
    assert(routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;
    SAEVRouteChangelist req1Changelist = routesContainer.tryAddRequest(1, &routesContainer.getOriginDepot(vehicleId),
                                                                       &routesContainer.getRequestDestination(0));
    std::cout << routesContainer.to_string(vehicleId) << std::endl << std::endl;
    assert(!routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;

    //Test changelist revert/apply
    req1Changelist.revertChanges();
    req1Changelist.applyChanges();
    req1Changelist.revertChanges();
    req0Changelist.revertChanges();
}

TEST(BestInsertionQueueDebug, DebugInstanceAlain) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "Instance_Alain_140624/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    //Vehicle 1 insertions
    BestInsertionQueue biQueue = routesContainer.getBestInsertionsQueue(0,0);
    routesContainer.tryAddRequest(0,&routesContainer.getOriginDepot(0),&routesContainer.getOriginDepot(0));
    routesContainer.getBestFeasibleInsertionsQueue(1,0);
    routesContainer.tryAddRequest(1, &routesContainer.getRequestOrigin(0), &routesContainer.getRequestOrigin(0));
    routesContainer.getBestFeasibleInsertionsQueue(2,0);
    SAEVRouteChangelist cl = routesContainer.tryAddRequest(2, &routesContainer.getRequestOrigin(1),
                                                           &routesContainer.getRequestDestination(1));
    routesContainer.getBestFeasibleInsertionsQueue(3,0);

    //Vehicle 2 insertions
    routesContainer.getBestFeasibleInsertionsQueue(5,1);
    routesContainer.tryAddRequest(5,&routesContainer.getOriginDepot(1),&routesContainer.getOriginDepot(1));
    routesContainer.getBestFeasibleInsertionsQueue(4,1);
    routesContainer.tryAddRequest(4,&routesContainer.getOriginDepot(1), &routesContainer.getRequestDestination(5));
    routesContainer.getBestFeasibleInsertionsQueue(3,1);
    routesContainer.tryAddRequest(3,&routesContainer.getOriginDepot(1), &routesContainer.getRequestOrigin(4));
    routesContainer.getBestFeasibleInsertionsQueue(0,1);
    routesContainer.getBestFeasibleInsertionsQueue(1,1);
    routesContainer.getBestFeasibleInsertionsQueue(2,1);
}

TEST(BestInsertionPerVehicleHeuristicDebug, DebugInstanceAlain) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "Instance_Alain_140624/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    //Vehicle 0 insertions (0,1,2)
    BestInsertionHeuristic::tryVehicleBestInsertion(0, 0, routesContainer);
    BestInsertionHeuristic::tryVehicleBestInsertion(1, 0, routesContainer);
    BestInsertionHeuristic::tryVehicleBestInsertion(2, 0, routesContainer);
    //Vehicle 1 insertions (3,4,5)
    BestInsertionHeuristic::tryVehicleBestInsertion(3, 1, routesContainer);
    BestInsertionHeuristic::tryVehicleBestInsertion(4, 1, routesContainer);
    BestInsertionHeuristic::tryVehicleBestInsertion(5, 1, routesContainer);
}

TEST(BestInsertionHeuristicDebug, DebugInstanceAlain2) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "Instance_Alain_140624_2/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    auto t0 = std::chrono::steady_clock::now();
    BestInsertionHeuristic::doBestRequestInsertionForRoute(0, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(1, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(2, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(3, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(4, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(5, routesContainer);
    auto t1 = std::chrono::steady_clock::now();
    auto d = t1 - t0;
    std::cout << d.count() << "\n";
}

TEST(BestInsertionHeuristicDebug, DebugInstanceAlain) {
    std::string instancesPath = "../../resources/test/instances/BestInsertionHeuristic/";
    std::string instanceFolder = "DebugWeight/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = Request::getRequestsFromFile(instancesPath + instanceFolder + requestsDatFile, graphFromSingleFile);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

    auto t0 = std::chrono::steady_clock::now();
    BestInsertionHeuristic::doBestRequestInsertionForRoute(0, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(1, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(2, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(3, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(4, routesContainer);
    BestInsertionHeuristic::doBestRequestInsertionForRoute(5, routesContainer);
    auto t1 = std::chrono::steady_clock::now();
    auto d = t1 - t0;
    std::cout << d.count() << "\n";
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}