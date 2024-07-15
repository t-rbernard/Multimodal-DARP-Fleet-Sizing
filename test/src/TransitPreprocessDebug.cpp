//
// Created by romain on 20/03/24.
//

#include "../lib/googletest/googletest/include/gtest/gtest.h"
#include "../../src/instance/graph/Graph.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathContainer.h"
#include "../../src/ShortestPath/Transit/TransitShortestPathPrecompute.h"

TEST(Transit_Preprocess_DEB, DebugFunction) {
    std::string instancesPath = "../../resources/test/instances/PT Shortest Path/";
    std::string instanceFolder = "contiguous_lines_debug_instance/";
    std::string datFile = "graph.dat";

    Graph graphFromSingleFile(instancesPath + instanceFolder + datFile);
    graphFromSingleFile.exportGraphToFile("../../resources/test/outputs/" + instanceFolder);
    TransitShortestPathContainer contiguousContainer(graphFromSingleFile.getNbNodes());
    for(auto& ptLine : graphFromSingleFile.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                contiguousContainer.addShortestPathCollection(i, startingTime, graphFromSingleFile.getNbNodes(),
                        TransitShortestPathPrecompute::executeAlgorithm(graphFromSingleFile, ptLine.getNode(i),startingTime));
                contiguousContainer.getShortestPathsFrom(i, startingTime - 1);
            }
        }
    }

    TransitShortestPathContainer crossingContainer(graphFromSingleFile.getNbNodes());
    Graph crossingLinesGraph(instancesPath + "multiple_crossing_lines_debug_instance/" + datFile);
    for(auto& ptLine : crossingLinesGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                crossingContainer.addShortestPathCollection(i, startingTime, crossingLinesGraph.getNbNodes(),
                        TransitShortestPathPrecompute::executeAlgorithm(crossingLinesGraph, ptLine.getNode(i),startingTime));
                crossingContainer.getShortestPathsFrom(i, startingTime - 1);
            }
        }
    }

    TransitShortestPathContainer cycleContainer(graphFromSingleFile.getNbNodes());
    Graph cyclingLineGraph(instancesPath + "cycling_line_debug_instance/" + datFile);
    for(auto& ptLine : cyclingLineGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                cycleContainer.addShortestPathCollection(i, startingTime, cyclingLineGraph.getNbNodes(),
                        TransitShortestPathPrecompute::executeAlgorithm(cyclingLineGraph, ptLine.getNode(i),startingTime));
                cycleContainer.getShortestPathsFrom(i, startingTime - 1);
            }
        }
    }


    TransitShortestPathContainer multiCycleContainer(graphFromSingleFile.getNbNodes());
    Graph multipleCyclingLinesGraph(instancesPath + "multiple_cycling_lines_debug_instance/" + datFile);
    for(auto& ptLine : multipleCyclingLinesGraph.getPTLines()) {
        for(int i = 0; i < ptLine.size(); ++i) {
            for (auto& startingTime: ptLine.getTimetable(i)) {
                multiCycleContainer.addShortestPathCollection(i, startingTime, multipleCyclingLinesGraph.getNbNodes(),
                        TransitShortestPathPrecompute::executeAlgorithm(multipleCyclingLinesGraph, ptLine.getNode(i),startingTime));
                multiCycleContainer.getShortestPathsFrom(i, startingTime - 1);
            }
        }
    }
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}