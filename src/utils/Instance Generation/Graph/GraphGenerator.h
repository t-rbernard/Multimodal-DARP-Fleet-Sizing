//
// Created by romain on 13/06/24.
//

#ifndef GREEDYALGORITHM_GRAPHGENERATOR_H
#define GREEDYALGORITHM_GRAPHGENERATOR_H


#include "../../../instance/graph/Graph.h"
#include "PTLineGenerationParameters.h"

class GraphGenerator {
    /**
     * Overwrites the node vector in the given graph with data imported from the given file in a CSV-like format (status,x,y)
     * @param baseGraph The graph in which to update node data
     * @param nodeFile The file containing the required data. Format : (status,x,y)
     */
    static void importNodeFromCSV(Graph baseGraph, std::filesystem::path nodeFile);
    static void generateNodes(Graph baseGraph, uint maxX, uint maxY, uint nbNodes) {

    }
    static void importDistanceMatrixFromCSV(Graph baseGraph, std::filesystem::path nodeFile);
    static void generateDistanceMatrix(Graph baseGraph, std::filesystem::path nodeFile);
    static void importPTNetworkFromCSV(Graph baseGraph, std::filesystem::path ptNetworkFile);
    static void generatePTNetwork(Graph baseGraph, PTLineGenerationParameters parameters);
};


#endif //GREEDYALGORITHM_GRAPHGENERATOR_H
