//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../../src/instance/Instance.h"
#include "../../src/routes/vehicle/SAEVRoute.h"

TEST(TransitPreprocessUnitTest, DebugFunction) {
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

    SAEVRouteChangelist req0Changelist = routesContainer.tryAddRequest(0, routesContainer.getOriginDepotIdx(1), routesContainer.getOriginDepotIdx(1));
    SAEVRouteChangelist req1Changelist = routesContainer.tryAddRequest(1, routesContainer.getOriginDepotIdx(1), routesContainer.getRequestOriginIdx(0));

    //Test changelist revert/apply
    req1Changelist.revertChanges();
    req1Changelist.applyChanges();
    req1Changelist.revertChanges();
    req0Changelist.revertChanges();
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}