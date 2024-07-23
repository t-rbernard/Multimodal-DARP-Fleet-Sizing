//
// Created by romain on 16/07/24.
//

#include <queue>
#include "VehicleShortestPathCalculation.h"
#include "VehiclePathState.h"
#include "../../utils/Constants.h"

std::vector<uint> VehicleShortestPathCalculation::computeShortestPathsFromNode(Graph& graph, size_t startingNodeIdx) {
    std::vector<uint> results{graph.getShortestSaevPaths()[startingNodeIdx]};
    std::vector<bool> mark(graph.getNbNodes(),false);
    std::priority_queue<VehiclePathState,std::vector<VehiclePathState>, std::greater<>> stateQueue{};

    //Init state queue with the current distances to check
    for(size_t i = 0; i < graph.getNbNodes(); ++i) {
        stateQueue.emplace(i, results[i]);
    }

    uint newDistance = INT32_MAX;
    VehiclePathState currentState;
    while(!stateQueue.empty()) {
        currentState = stateQueue.top();
        stateQueue.pop();
        //Only expand and add
        if(!mark[currentState.getNodeIndex()]) {
            //Considering order of iteration is by shortest path to starting node, it's fair to add the current state node as a closest station
            if(graph.getNbPTLines(currentState.getNodeIndex()) > 0 && graph.getNbClosestStations(startingNodeIdx) < Constants::MAX_TRANSIT_ENTRY_CANDIDATES) {
                graph.emplaceBackClosestStation(startingNodeIdx, currentState.getNodeIndex());
            }

            //Iterate over all possible nodes, as the graph is complete in the case of a distance matrix
            for(size_t i = 0; i < results.capacity(); ++i) { //FIXME:change iteration here to allow using edges
                newDistance = currentState.getInstant() + graph.getShortestSAEVPath(currentState.getNodeIndex(), i);
                if(newDistance < results[i]) {
                    stateQueue.emplace(i, newDistance);
                    results[i] = newDistance;
                }
            }
        }
    }

    return results;
}

MatrixShortestPathContainer VehicleShortestPathCalculation::computeShortestPathsForGraph(Graph &graph) {
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
