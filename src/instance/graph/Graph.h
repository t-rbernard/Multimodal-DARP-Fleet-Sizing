//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_GRAPH_H
#define GREEDYALGORITHM_GRAPH_H


#include <utility>
#include <string>
#include <filesystem>

#include "Node.h"
#include "Edge.h"

class Graph {
private:
    std::vector<Node> nodesVector;
    std::vector<Edge> edgesVector;
    std::vector<Line> transitLines;
public:
    [[nodiscard]] const std::vector<Node> &getNodesVector() const {
        return nodesVector;
    }

    [[nodiscard]] const std::vector<Edge> &getEdgesVector() const {
        return edgesVector;
    }

    [[nodiscard]] const std::vector<Line> &getPTLines() const {
        return transitLines;
    }

    std::vector<Edge> addEdge(Edge& edge) {
        edgesVector.push_back(edge);
        return edgesVector;
    }

    std::vector<Node> addNode(Node& node) {
        nodesVector.push_back(node);
        return nodesVector;
    }

    explicit Graph(const std::string& datFilePath);
    Graph(const std::string& nodeFilePath, const std::string& edgeFilePath, const std::string& ptLineFilePath);

    std::vector<Line> addLine(Line& line);
    void exportGraphToFiles(std::filesystem::path exportFolderPath);
};


#endif //GREEDYALGORITHM_GRAPH_H
