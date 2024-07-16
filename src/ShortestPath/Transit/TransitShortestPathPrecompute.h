//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H

#include "TransitShortestPath.h"
#include "../../instance/graph/Graph.h"
#include "TransitStateContainer.h"

class TransitShortestPathPrecompute {
public:
    static TransitStateContainer executeAlgorithm(const Graph& graph, int nodeIndex, int instant);
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
