//
// Created by romain on 16/07/24.
//

#include <queue>
#include "VehicleShortestPathCalculation.h"
#include "VehiclePathState.h"

std::vector<uint> VehicleShortestPathCalculation::computeShortestPathsFromNode(const Graph& graph, size_t startingNodeIdx) {
    std::vector<uint> results{graph.getShortestSaevPaths()[startingNodeIdx]};
    std::priority_queue<VehiclePathState> stateQueue{};
    stateQueue.emplace(startingNodeIdx, 0);

    uint newDistance = INT32_MAX;
    VehiclePathState currentState;
    while(!stateQueue.empty()) {
        currentState = stateQueue.top();
        stateQueue.pop();
        //Iterate over all possible nodes, as the graph is complete in the case of a distance matrix
        for(size_t i = 0; i < results.capacity(); ++i) {
            newDistance = currentState.getInstant() + graph.getShortestSAEVPath(currentState.getNodeIndex(), i);
            if(newDistance < results[i]) {
                stateQueue.emplace(i, newDistance);
                results[i] = newDistance;
            }
        }
    }

    return results;
}

MatrixShortestPathContainer VehicleShortestPathCalculation::computeShortestPathsForGraph(const Graph &graph) {
    std::vector<std::vector<uint>> results;
    results.resize(graph.getNbNodes());
    for(size_t i = 0; i < graph.getNbNodes(); ++i) {
        results[i].resize(graph.getNbNodes());
        std::ranges::move(computeShortestPathsFromNode(graph, i), results[i].begin());
    }

    return MatrixShortestPathContainer(results);
}

void VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(Graph &graph) {
    MatrixShortestPathContainer results = computeShortestPathsForGraph(graph);
    graph.setShortestSaevPaths(results.getDistanceMatrix());
}

ClosestDestinationsContainer
VehicleShortestPathCalculation::getClosestPTNodesFromX(const Graph &graph, size_t startingNodeIdx) {
    ClosestDestinationsContainer closestDestinationsContainer(startingNodeIdx);
    for(size_t destinationIdx=0; destinationIdx < graph.getNbNodes(); ++destinationIdx) {
        if(graph.getNbPTLines(destinationIdx) > 0) {
            closestDestinationsContainer.addDestination(destinationIdx,
                                                        graph.getShortestSAEVPath(startingNodeIdx, destinationIdx));
        }
    }
    return closestDestinationsContainer;
}
