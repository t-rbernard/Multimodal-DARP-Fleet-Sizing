//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
#define GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H


#include "MatrixShortestPathContainer.h"
#include "ClosestDestinationsContainer.h"
#include "VehiclePathState.h"

class Graph;
class VehicleShortestPathCalculation {
public:
    /**
     * Computes shortest paths from the given graph's distance matrix
     * @param graph
     */
    static void computeAndUpdateShortestPathsForGraph(Graph& graph);
    /**
     * Computes and returns an updated distance matrix from the given graph's distance matrix computed via Dijsktra's algorithm
     * @param graph
     * @return A matrix distances container
     */
    static MatrixShortestPathContainer computeShortestPathsForGraph(Graph& graph);
    /**
     * Computes the shortest paths to all the nodes in the graph from the given starting node.
     * If a node is unreachable its default distance is INT32_MAX. Distance from the starting node to itself is 0
     * @param graph
     * @param startingNodeIdx
     * @return a vector indexed by all the nodes with the sortest path's duration from the starting node to the destination node index
     */
    static std::vector<uint> computeShortestPathsFromNode(Graph &graph, size_t startingNodeIdx, bool useEdges);
    /**
     * Returns an ordered set of closest PT SAV-compatible nodes from a given starting node index.
     * Standard use case expects that the starting node index is a request origin
     * @param graph
     * @param startingNodeIdx
     * @return An ordered set containing PT nodes accessible via SAV from the given starting node index
     */
    static ClosestDestinationsContainer getClosestPTNodesFromX(const Graph& graph, size_t startingNodeIdx);
private:
    static void expandStatesViaEdges(const VehiclePathState& currentState, std::vector<uint> &results,
                                     std::priority_queue<VehiclePathState, std::vector<VehiclePathState>, std::greater<>> &stateQueue, const Graph& graph);
    static void expandStatesViaMatrix(const VehiclePathState& currentState, std::vector<uint> &results,
                                      std::priority_queue<VehiclePathState, std::vector<VehiclePathState>, std::greater<>> &stateQueue, const Graph& graph);
};
#include "../../instance/graph/Graph.h"


#endif //GREEDYALGORITHM_VEHICLESHORTESTPATHCALCULATION_H
