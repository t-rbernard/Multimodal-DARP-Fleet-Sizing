//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_GRAPH_H
#define GREEDYALGORITHM_GRAPH_H


#include <utility>
#include <string>
#include <random>
#include <filesystem>

#include "Node.h"
#include "Edge.h"
#include "../../ShortestPath/Vehicle/VehicleShortestPathCalculation.h"
#include "../../utils/Constants.h"
#include "../../ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../ShortestPath/Transit/TransitShortestPathPrecompute.h"

class DATRow;
class Graph {
private:
    std::vector<Node> nodesVector; //The full list of nodes created in the graph
    std::vector<Edge> edgesVector;
    std::vector<Line> transitLines;
    std::vector<std::vector<uint>> shortestSAEVPaths;
    TransitShortestPathContainer transitShortestPaths = TransitShortestPathContainer(0);
    size_t _depotNodeIdx{0}; //Index of the depot node, defaults to 0, the first node created in the graph

    /**
     * For every LineStop on every node of the graph, verify the node returned by looking into LineStop->Line(stopIdx)
     * is the same as the node where the LineStop is defined
     * @return True if @this is properly referenced in the Line object at the expected index
     */
    bool checkLineToNodeLinks();
    void parseNodeRow(const DATRow& row);
    void parseEdgeRow(const DATRow& row);
    void parseLineRandomizedSchedule(const DATRow &row, std::mt19937 rng,
                                     std::uniform_int_distribution<uint32_t> travelTimeDistribution,
                                     std::uniform_int_distribution<uint32_t> startTimeDistribution);

public:
    [[nodiscard]] const std::vector<Node> &getNodesVector() const {
        return nodesVector;
    }

    [[nodiscard]] const Node &getNode(size_t nodeIndex) const {
        return nodesVector[nodeIndex];
    }

    [[nodiscard]] size_t getNbNodes() const {
        return nodesVector.size();
    }

    [[nodiscard]] const std::vector<LineStop> & getPTLinesSet(size_t nodeIndex) const {
        return nodesVector[nodeIndex].getPTLinesSet();
    }

    [[nodiscard]] size_t getNbPTLines(size_t nodeIndex) const {
        return nodesVector[nodeIndex].getPTLinesSet().size();
    }

    [[nodiscard]] size_t getDepotNodeIdx() const;
    void setDepotNodeIdx(size_t depotNodeIdx);

    /**
     * @return The graph's edge vector
     */
    [[nodiscard]] const std::vector<Edge> &getEdgesVector() const {
        return edgesVector;
    }
    /**
     * @return Const ref to the edge at the given index
     */
    [[nodiscard]] const Edge &getEdge(size_t edgeIndex) const {
        return edgesVector[edgeIndex];
    }

    [[nodiscard]] size_t getNbIncomingEdges(size_t nodeIndex) const {
        return nodesVector[nodeIndex].getIncomingEdges().size();
    }

    [[nodiscard]] size_t getNbOutgoingEdges(size_t nodeIndex) const {
        return nodesVector[nodeIndex].getOutgoingEdges().size();
    }

    //TODO: try and define an Edge iterator over a given node's incoming/outgoing

    [[nodiscard]] size_t getNbEdges(size_t nodeIndex) const {
        return nodesVector[nodeIndex].getIncomingEdges().size() + nodesVector[nodeIndex].getOutgoingEdges().size();
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
    const std::vector<Edge>& addEdge(Edge const& edge) {
        edgesVector.push_back(edge);
        return edgesVector;
    }

    /**
     * Pushes a reference to the given node at the end of the graph's node vector
     * @param node The node to push in the graph's node vector
     * @return The node vector after having added the node
     */
    const std::vector<Node>& addNode(Node const& node) {
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
    const std::vector<Line>& addLine(const Line &line);
    /**
     * Export graph data (nodes, edges, transit lines) to files to plot graph data with Python
     * The function creates the folders if need be, and will overwrite existing files if data has been outputted to this folder before
     * @param exportFolderPath A path to a folder where we can export edges.txt, nodes.txt and ptlines.txt
     */
    void exportGraphToFile(const std::filesystem::path& exportFolderPath);

    /**
     * Executes defined check functions on every node and transit line in the graph
     * @return True if all checks were successful, false otherwise
     */
    bool check();

    /**
     * Adds a new edge at the back of the edgesVector, and properly links this edge to its entry and exit nodes
     * @param edgeStartNodeIndex Index of the node serving as a starting point for this edge
     * @param edgeEndNodeIndex Index of the node serving as an exit point for this edge
     * @param length The length of this edge (in minutes)
     */
    void createAndAddEdge(size_t edgeStartNodeIndex, size_t edgeEndNodeIndex, double length);

    [[nodiscard]] const std::vector<std::vector<uint>> &getShortestSaevPaths() const;

    void setShortestSaevPaths(const std::vector<std::vector<uint>> &shortestSaevPaths);

    [[nodiscard]] uint getShortestSAEVPath(size_t startingNodeIdx, size_t destinationNodeIdx) const { return shortestSAEVPaths[startingNodeIdx][destinationNodeIdx]; }

    void parseDistanceMatrix(std::ifstream &infile, DATRow currentRow);

    void emplaceBackClosestStation(size_t nodeIdx, size_t stationNodeIdx);

    const size_t getNbClosestStations(size_t nodeIdx);

    void computeAndUpdateShortestPathsMatrix() {
        VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(*this, false);
    }

    void computeAndUpdateShortestPathsMatrix(bool useEdges) {
        VehicleShortestPathCalculation::computeAndUpdateShortestPathsForGraph(*this, useEdges);
    }

    void computeAndUpdateClosestStationsForNode(size_t nodeIdx);
    void computeAndUpdateClosestStations();
    void computeAndUpdateShortestTransitPaths();

    void linkAllPTNodes();
};


#endif //GREEDYALGORITHM_GRAPH_H
