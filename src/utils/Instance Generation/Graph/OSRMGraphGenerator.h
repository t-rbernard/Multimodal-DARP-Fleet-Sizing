//
// Created by romain on 12/06/24.
//

#ifndef GREEDYALGORITHM_OSRMGRAPHGENERATOR_H
#define GREEDYALGORITHM_OSRMGRAPHGENERATOR_H


#include "../../../instance/graph/Graph.h"

class OSRMGraphGenerator {
    static void initOSRMEngine();
    static Graph createMatrixFromOSRM(Graph graph);
};


#endif //GREEDYALGORITHM_OSRMGRAPHGENERATOR_H
