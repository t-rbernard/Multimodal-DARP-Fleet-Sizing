//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H

#include "../TimeDependentShortestPathContainer.h"
#include "TransitShortestPath.h"
#include "TransitPrecomputeIterationContainer.h"
#include "../../instance/graph/Graph.h"

class TransitShortestPathPrecompute {
    static ShortestPathContainer<TransitShortestPath> formatResults(TimeDependentShortestPathContainer<TransitShortestPath> container);
    static TransitPrecomputeIterationContainer executeAlgorithm(const Graph& graph, int nodeIndex, int instant);
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
