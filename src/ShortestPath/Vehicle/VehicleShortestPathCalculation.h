//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
#define GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H


#include "MatrixShortestPathContainer.h"
#include "../../instance/graph/Graph.h"

class VehicleShortestPathCalculation {
public:
    static void computeAndUpdateShortestPathsForGraph(Graph& graph);
    static MatrixShortestPathContainer computeShortestPathsForGraph(const Graph& graph);
    static std::vector<uint> computeShortestPathsFromNode(const Graph& graph, size_t startingNodeIdx);

};


#endif //GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
