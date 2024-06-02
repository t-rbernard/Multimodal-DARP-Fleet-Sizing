//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../../src/instance/Instance.h"

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
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}