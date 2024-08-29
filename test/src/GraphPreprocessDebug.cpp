//
// Created by romain on 24/07/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/ShortestPath/Vehicle/VehicleShortestPathCalculation.h"


TEST(GraphPreprocessDebug, DebugBookInstance) {
    std::string instancesPath = "../../resources/test/instances/graph preprocess/";
    std::string instanceFolder = "book example/";
    std::string graphDatFile = "graph.dat";

    //Parse graph
    Graph graph(instancesPath + instanceFolder + graphDatFile);
    VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(graph, false);

    std::string exportFolder = "../../resources/test/outputs/preprocess_debug/";
    graph.exportGraphToFile(exportFolder + instanceFolder);
}
