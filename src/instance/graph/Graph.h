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
    std::vector<Node> nodesVector; //The full list of nodes created in the graph
    std::vector<Edge> edgesVector;
    std::vector<Line> transitLines;

    /**
     * For every LineStop on every node of the graph, verify the node returned by looking into LineStop->Line(stopIdx)
     * is the same as the node where the LineStop is defined
     * @return True if @this is properly referenced in the Line object at the expected index
     */
    bool checkLineToNodeLinks();
public:
    [[nodiscard]] const std::vector<Node> &getNodesVector() const {
        return nodesVector;
    }

    /**
     * @return The graph's edge vector
     */
    [[nodiscard]] const std::vector<Edge> &getEdgesVector() const {
        return edgesVector;
    }

    /**
     * @return A vector containing the list of transit lines defined in our graph (effectively G^TP)
     */
    [[nodiscard]] const std::vector<Line> &getPTLines() const {
        return transitLines;
    }

    /**
     * Pushes a reference to the given edge at the end of the graph's edge vector
     * @param edge The edge to push in the graph's edge vector
     * @return The edge vector after having added the edge
     */
    std::vector<Edge> addEdge(Edge& edge) {
        edgesVector.push_back(edge);
        return edgesVector;
    }

    /**
     * Pushes a reference to the given node at the end of the graph's node vector
     * @param node The node to push in the graph's node vector
     * @return The node vector after having added the node
     */
    std::vector<Node> addNode(Node& node) {
        nodesVector.push_back(node);
        return nodesVector;
    }

    /**
     * Creates a Graph and fills it with data from a single dat file with :
     * nodes, edges and public transit data. Each separated by a #comment line.
     * Each object is a line in the file
     * @param datFilePath relative or absolute path to the dat file to import objects from
     */
    explicit Graph(const std::string& datFilePath);
    /**
     * Creates a Graph and fills it with data from individual files for each object type :
     * nodes, edges and public transit data. Lines starting by # will be considered comments and skipped.
     * Each object is a line in the file
     * @param nodeFilePath The file containing node data (format for each line : status,x,y)
     * @param edgeFilePath The file containing edge data (format for each line : start_node_index,end_node_index,edge_length)
     * @param ptLineFilePath
     */
    Graph(const std::string& nodeFilePath, const std::string& edgeFilePath, const std::string& ptLineFilePath);

    /**
     * Adds a new Line to the graph.
     * @param line The Line to add. It's expected to be fully filled with its nodes schedules before adding to the graph
     * @return The updated vector containing all lines of the graph
     */
    std::vector<Line> addLine(Line& line);
    /**
     * Export graph data (nodes, edges, transit lines) to files to plot graph data with Python
     * The function creates the folders if need be, and will overwrite existing files if data has been outputted to this folder before
     * @param exportFolderPath A path to a folder where we can export edges.txt, nodes.txt and ptlines.txt
     */
    void exportGraphToFiles(std::filesystem::path exportFolderPath);

    /**
     * Executes defined check functions on every node and transit line in the graph
     * @return True if all checks were successful, false otherwise
     */
    bool check();

};


#endif //GREEDYALGORITHM_GRAPH_H
