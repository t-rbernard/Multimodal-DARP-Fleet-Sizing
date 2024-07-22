//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
#define GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H


#include "MatrixShortestPathContainer.h"
#include "../../instance/graph/Graph.h"
#include "ClosestDestinationsContainer.h"

class VehicleShortestPathCalculation {
public:
    static void computeAndUpdateShortestPathsForGraph(Graph& graph);
    static MatrixShortestPathContainer computeShortestPathsForGraph(const Graph& graph);
    static std::vector<uint> computeShortestPathsFromNode(const Graph& graph, size_t startingNodeIdx);
    /**
     * Returns an ordered set of closest PT SAV-compatible nodes from a given starting node index.
     * Standard use case expects that the starting node index is a request origin
     * @param graph
     * @param startingNodeIdx
     * @return An ordered set containing PT nodes accessible via SAV from the given starting node index
     */
    static ClosestDestinationsContainer getClosestPTNodesFromX(const Graph& graph, size_t startingNodeIdx);
};


#endif //GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
