//
// Created by rbernard on 20/02/2024.
//

#include "TransitShortestPathPrecompute.h"
#include "TransitStateContainer.h"

TransitStateContainer TransitShortestPathPrecompute::executeAlgorithm(const Graph& graph, int nodeIndex, int instant) {
    TransitStateContainer solutionsContainer{};
    solutionsContainer.resizeSolutionsVector(graph.getNodesVector().size());
    for(nodeIndex = 0; nodeIndex < graph.getNodesVector().size(); ++nodeIndex) {

    }

    return solutionsContainer;
}
