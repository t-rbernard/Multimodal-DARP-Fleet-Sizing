//
// Created by romain on 22/01/24.
//

#ifndef GREEDYALGORITHM_GRAPH_H
#define GREEDYALGORITHM_GRAPH_H


#include <utility>
#include <string>

#include "Node.h"
#include "Edge.h"

class Graph {
private:
    std::vector<Node> nodesVector;
    std::vector<Edge> edgesVector;
public:
    const std::vector<Node> &getNodesVector() const {
        return nodesVector;
    }

    const std::vector<Edge> &getEdgesVector() const {
        return edgesVector;
    }

    std::vector<Edge> addEdge(Edge& edge) {
        edgesVector[edgesVector.size()] = edge;
        return edgesVector;
    }

    std::vector<Node> addNode(Node& node) {
        nodesVector[edgesVector.size()] = node;
        return nodesVector;
    }

    Graph(std::string filePath);
};


#endif //GREEDYALGORITHM_GRAPH_H
