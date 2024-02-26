//
// Created by rbernard on 30/01/2024.
//
#include <iostream>
#include "../src/instance/graph/Graph.h"
#include "../src/ShortestPath/Transit/TransitStateContainer.h"

int main() {
//    Graph give_me_a_name("../resources/test/instances/basic_debug_instance/nodes.csv",
//          "../resources/test/instances/basic_debug_instance/edges.csv",
//          "../resources/test/instances/basic_debug_instance/PT_lines.csv");

    Graph graphFromSingleFile("../resources/test/instances/basic_debug_instance/graph.dat");
    graphFromSingleFile.exportGraphToFiles("../resources/test/outputs/basic_debug_instance/");
    TransitStateContainer stateContainer;
    stateContainer.pushNewState(0);
    return 0;
}