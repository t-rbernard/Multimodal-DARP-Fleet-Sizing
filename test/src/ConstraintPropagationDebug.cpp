//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"
#include "../../src/utils/RequestsGenerator.h"

TEST(ConstraintPropagationDebug, DebugBaseInstance) {
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
    SAEVRouteChangelist req0Changelist = routesContainer.tryAddRequest(0, routesContainer.getOriginDepotIdx(vehicleId), routesContainer.getOriginDepotIdx(1));
    std::cout << routesContainer.to_string(vehicleId) << std::endl;
    assert(routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;
    SAEVRouteChangelist req1Changelist = routesContainer.tryAddRequest(1, routesContainer.getOriginDepotIdx(vehicleId), routesContainer.getRequestDestinationIdx(0));
    std::cout << routesContainer.to_string(vehicleId) << std::endl << std::endl;
    assert(!routesContainer.checkRouteTimeWindows(vehicleId));
    std::cout << "------------------------------------------------------------------" << std::endl;

    //Test changelist revert/apply
    req1Changelist.revertChanges();
    req1Changelist.applyChanges();
    req1Changelist.revertChanges();
    req0Changelist.revertChanges();
}

TEST(ConstraintPropagationDebug, DebugRequestGeneration) {
    std::string instancesPath = "../../resources/test/instances/Constraint Propagation/";
    std::string instanceFolder = "basic_debug_instance/";
    std::string graphDatFile = "graph.dat";
    std::string requestsDatFile = "requests.dat";

    //Parse graph
    Graph graphFromSingleFile(instancesPath + instanceFolder + graphDatFile);
    std::vector<Request> requests = RequestsGenerator::generateRequests(graphFromSingleFile, 100, 110620241720);
    std::vector<Request> requestsParameterized = RequestsGenerator::generateRequests(graphFromSingleFile, 100, 1.5, 15, 15, 480, 600, 110620241739);

    assert(requests.size() == 100);
    assert(requestsParameterized.size() == 100);

    //Init instance
    Instance instance(requests,graphFromSingleFile,4);
    SAEVRoute routesContainer(graphFromSingleFile, requests);

}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}