//
// Created by Bebs on 30/01/2024.
//
#include <iostream>
#include "../src/instance/graph/Graph.h"

int main() {
    Graph("../resources/test/instances/basic_debug_instance/nodes.csv",
          "../resources/test/instances/basic_debug_instance/edges.csv",
          "../resources/test/instances/basic_debug_instance/PT_lines.csv");
    return 0;
}