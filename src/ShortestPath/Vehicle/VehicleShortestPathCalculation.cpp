//
// Created by romain on 16/07/24.
//

#include <queue>
#include "VehicleShortestPathCalculation.h"
#include "VehiclePathState.h"
#include "../../utils/Constants.h"

std::vector<uint>
VehicleShortestPathCalculation::computeShortestPathsFromNode(Graph &graph, size_t startingNodeIdx, bool useEdges) {
    std::vector<uint> results;
    results.reserve(graph.getNbNodes());

    if(!useEdges)
        results = graph.getShortestSaevPaths()[startingNodeIdx];

    std::vector<bool> mark(graph.getNbNodes(),false);
    std::priority_queue<VehiclePathState,std::vector<VehiclePathState>, std::greater<>> stateQueue{};

    //Init state queue with the current distances to check
    for(size_t i = 0; i < graph.getNbNodes(); ++i) {
        stateQueue.emplace(i, results[i]);
    }

    VehiclePathState currentState;
    while(!stateQueue.empty()) {
        currentState = stateQueue.top();
        stateQueue.pop();
        //Only expand and add
        if(!mark[currentState.getNodeIndex()]) {
            mark[currentState.getNodeIndex()] = true;

            //Considering order of iteration is by shortest path to starting node, it's fair to add the current state node as a closest station
            if(graph.getNbPTLines(currentState.getNodeIndex()) > 0 && graph.getNbClosestStations(startingNodeIdx) < Constants::MAX_CLOSEST_STATIONS_CANDIDATES) {
                graph.emplaceBackClosestStation(startingNodeIdx, currentState.getNodeIndex());
            }

            //Iterate over all possible nodes, as the graph is complete in the case of a distance matrix
            if (useEdges) {
                expandStatesViaEdges(currentState, results, stateQueue, graph);
            } else {
                expandStatesViaMatrix(currentState, results, stateQueue, graph);
            }
        }
    }

    return results;
}

MatrixShortestPathContainer VehicleShortestPathCalculation::computeShortestPathsForGraph(Graph &graph) {
    return computeShortestPathsForGraph(graph, false);
}

MatrixShortestPathContainer VehicleShortestPathCalculation::computeShortestPathsForGraph(Graph &graph, bool useEdges) {
    std::vector<std::vector<uint>> results;
    results.resize(graph.getNbNodes());
    for(size_t i = 0; i < graph.getNbNodes(); ++i) {
        results[i].resize(graph.getNbNodes());
        std::ranges::move(computeShortestPathsFromNode(graph, i, useEdges), results[i].begin());
    }

    return MatrixShortestPathContainer(results);
}

void VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(Graph &graph) {
    computeAndUpdateShortestPathsForGraph(graph, false);
}

void VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(Graph &graph, bool useEdges) {
    MatrixShortestPathContainer results = computeShortestPathsForGraph(graph, useEdges);
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

void VehicleShortestPathCalculation::expandStatesViaMatrix(const VehiclePathState& currentState, std::vector<uint> &results,
                                                           std::priority_queue<VehiclePathState, std::vector<VehiclePathState>, std::greater<>> &stateQueue,
                                                           const Graph& graph) {
    uint newDistance = INT32_MAX;
    for(size_t i = 0; i < results.capacity(); ++i) {
        newDistance = currentState.getInstant() + graph.getShortestSAEVPath(currentState.getNodeIndex(), i);
        if(newDistance < results[i]) {
            stateQueue.emplace(i, newDistance);
            results[i] = newDistance;
        }
    }
}

void VehicleShortestPathCalculation::expandStatesViaEdges(const VehiclePathState &currentState, std::vector<uint> &results,
                                                          std::priority_queue<VehiclePathState, std::vector<VehiclePathState>, std::greater<>> &stateQueue,
                                                          const Graph& graph) {
    Edge edge;
    uint newDistance = INT32_MAX;
    for(const auto& edgeIndex : graph.getNode(currentState.getNodeIndex()).getOutgoingEdges()) {
        edge = graph.getEdgesVector()[edgeIndex];
        newDistance = currentState.getInstant() + edge.getLength();
        if(newDistance < results[edge.getEndNodeIdx()]) {
            stateQueue.emplace(edge.getEndNodeIdx(), newDistance);
            results[edge.getEndNodeIdx()] = newDistance;
        }
    }
}
