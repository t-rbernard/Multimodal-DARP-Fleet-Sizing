//
// Created by romain on 11/06/24.
//

#include <filesystem>
#include <fstream>
#include "RequestsGenerator.h"
#include "RequestGenerationParameters.h"

namespace fs = std::filesystem;

std::vector<Request>
RequestsGenerator::generateRequests(const Graph &graph, RequestGenerationParameters generationParameters) {
    size_t requestAmount = generationParameters.getRequestAmount();
    double deltaRatio = generationParameters.getDeltaRatio();
    double deltaMinDuration = generationParameters.getDeltaMinDuration();
    uint timeWindowWidth = generationParameters.getTimeWindowWidth();

    std::vector<Request> requests;
    requests.reserve(requestAmount);
    std::mt19937 rng(generationParameters.getRngSeed());
    std::uniform_int_distribution<uint32_t> requestTWStartDistribution(generationParameters.getPeriodStartTime() + timeWindowWidth,
                                                                       generationParameters.getPeriodEndTime() - timeWindowWidth); //reduce period window time to account for TW width

    std::uniform_int_distribution<ulong> nodeDistribution(0, graph.getNbNodes() - 1);

    //Init request variables
    uint deltaTime;
    uint requestTWStart;
    ulong originNodeIdx, destinationNodeIdx;
    for(size_t i = 0; i < requestAmount; ++i) {
        //Generate new values
        requestTWStart = requestTWStartDistribution(rng);
        originNodeIdx = nodeDistribution(rng);
        destinationNodeIdx = nodeDistribution(rng);
        while (destinationNodeIdx == originNodeIdx) { //retry random node as much as needed for origin to be different from destination
            destinationNodeIdx = nodeDistribution(rng);
        }
        //Set delta time with a guaranteed leeway compared to the shortest path
        deltaTime = graph.getShortestSAEVPath(originNodeIdx, destinationNodeIdx);
        if(floor(deltaTime * (deltaRatio - 1)) < deltaMinDuration) {
            deltaTime = deltaTime + deltaMinDuration;
        } else {
            deltaTime = floor(deltaTime * deltaRatio);
        }
        requests.emplace_back(originNodeIdx, destinationNodeIdx,
                              TimeWindow(requestTWStart, requestTWStart + timeWindowWidth), //TODO : could be replaced with a min/max width ?
                              deltaTime, 1);
    }
    return requests;
}

std::vector<Request>
RequestsGenerator::generateAndExportRequests(const fs::path &exportFolderPath, const Graph &graph, const RequestGenerationParameters &generationParameters) {
    std::vector<Request> generatedRequests = generateRequests(graph, generationParameters);
    //Write requests to files
    fs::create_directories(exportFolderPath);
    std::ofstream outfileGraph(exportFolderPath.string() + "requests.txt", std::ofstream::out | std::ofstream::trunc); //open and clear file if it already existed
    for(Request request : generatedRequests) {
        outfileGraph << request.to_string_export() << std::endl;
    }
    outfileGraph.close();
    //Export parameters to file
    std::ofstream outfileParams(exportFolderPath.string() + "requestParameters.txt", std::ofstream::out | std::ofstream::trunc); //open and clear file if it already existed
    outfileParams << generationParameters;
    outfileParams.close();
    return generatedRequests;
}
