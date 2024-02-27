//
// Created by rbernard on 30/01/2024.
//
#include <iostream>
#include "../src/instance/graph/Graph.h"
#include "../src/ShortestPath/Transit/TransitStateContainer.h"
#include "../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"

int main() {
//    Graph give_me_a_name("../resources/test/instances/basic_debug_instance/nodes.csv",
//          "../resources/test/instances/basic_debug_instance/edges.csv",
//          "../resources/test/instances/basic_debug_instance/PT_lines.csv");

//    std::string instanceFolder = "basic_debug_instance/";
    std::string instanceFolder = "two_lines_debug_instance/";
    std::string datFile = "graph.dat";

    Graph graphFromSingleFile("../resources/test/instances/" + instanceFolder + datFile);
    graphFromSingleFile.exportGraphToFiles("../resources/test/outputs/" + instanceFolder);
    for(auto& ptLine : graphFromSingleFile.getPTLines()) {
        for(int i = 0; i < ptLine.getNodes().size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                TransitShortestPathPrecompute::executeAlgorithm(graphFromSingleFile, ptLine.getNode(i),startingTime);
            }
        }
    }
    return 0;
}