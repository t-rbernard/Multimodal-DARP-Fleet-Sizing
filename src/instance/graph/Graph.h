//
// Created by rbernard on 22/01/24.
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
    [[nodiscard]] const std::vector<Node> &getNodesVector() const {
        return nodesVector;
    }

    [[nodiscard]] const std::vector<Edge> &getEdgesVector() const {
        return edgesVector;
    }

    std::vector<Edge> addEdge(Edge& edge) {
        edgesVector.push_back(edge);
        return edgesVector;
    }

    std::vector<Node> addNode(Node& node) {
        nodesVector.push_back(node);
        return nodesVector;
    }

    Graph(std::string nodeFilePath, std::string edgeFilePath, std::string ptLineFilePath);
};


#endif //GREEDYALGORITHM_GRAPH_H
