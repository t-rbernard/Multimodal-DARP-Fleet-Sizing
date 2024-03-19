//
// Created by rbernard on 30/01/2024.
//
#include <iostream>
#include "../src/instance/graph/Graph.h"
#include "../src/ShortestPath/Transit/TransitStateContainer.h"
#include "../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"
#include "../src/ShortestPath/Transit/TransitShortestPathContainer.h"

int main() {
//    Graph give_me_a_name("../resources/test/instances/basic_debug_instance/nodes.csv",
//          "../resources/test/instances/basic_debug_instance/edges.csv",
//          "../resources/test/instances/basic_debug_instance/PT_lines.csv");

//    std::string instanceFolder = "basic_debug_instance/";
    std::string instanceFolder = "contiguous_lines_debug_instance/";
    std::string datFile = "graph.dat";

    Graph graphFromSingleFile("../resources/test/instances/" + instanceFolder + datFile);
    graphFromSingleFile.exportGraphToFiles("../resources/test/outputs/" + instanceFolder);
    TransitShortestPathContainer contiguousContainer(graphFromSingleFile.getNbNodes());
    for(auto& ptLine : graphFromSingleFile.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                contiguousContainer.addShortestPathCollection(i, startingTime, graphFromSingleFile.getNbNodes(),
                                                              TransitShortestPathPrecompute::executeAlgorithm(graphFromSingleFile, ptLine.getNode(i),startingTime));
                contiguousContainer.getShortestPathsFromTime(i, startingTime - 1);
            }
        }
    }

    TransitShortestPathContainer crossingContainer(graphFromSingleFile.getNbNodes());
    Graph crossingLinesGraph("../resources/test/instances/multiple_crossing_lines_debug_instance/" + datFile);
    for(auto& ptLine : crossingLinesGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                crossingContainer.addShortestPathCollection(i, startingTime, crossingLinesGraph.getNbNodes(),
                                                              TransitShortestPathPrecompute::executeAlgorithm(crossingLinesGraph, ptLine.getNode(i),startingTime));
                crossingContainer.getShortestPathsFromTime(i, startingTime - 1);
            }
        }
    }

    TransitShortestPathContainer cycleContainer(graphFromSingleFile.getNbNodes());
    Graph cyclingLineGraph("../resources/test/instances/cycling_line_debug_instance/" + datFile);
    for(auto& ptLine : cyclingLineGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                cycleContainer.addShortestPathCollection(i, startingTime, cyclingLineGraph.getNbNodes(),
                                                              TransitShortestPathPrecompute::executeAlgorithm(cyclingLineGraph, ptLine.getNode(i),startingTime));
                cycleContainer.getShortestPathsFromTime(i, startingTime - 1);
            }
        }
    }


    TransitShortestPathContainer multiCycleContainer(graphFromSingleFile.getNbNodes());
    Graph multipleCyclingLinesGraph("../resources/test/instances/multiple_cycling_lines_debug_instance/" + datFile);
    for(auto& ptLine : multipleCyclingLinesGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                multiCycleContainer.addShortestPathCollection(i, startingTime, multipleCyclingLinesGraph.getNbNodes(),
                                                               TransitShortestPathPrecompute::executeAlgorithm(multipleCyclingLinesGraph, ptLine.getNode(i),startingTime));
                multiCycleContainer.getShortestPathsFromTime(i, startingTime - 1);
            }
        }
    }

    return 0;
}