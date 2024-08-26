//
// Created by romain on 13/03/24.
//

#include <algorithm>
#include "TransitShortestPathContainer.h"
#include "TransitStateContainer.h"

void TransitShortestPathContainer::addShortestPathCollection(size_t startNodeIndex,
                                                             const std::pair<size_t , std::vector<TransitShortestPath>>& shortestPathList) {
    container[startNodeIndex].emplace_back(shortestPathList);
}

void TransitShortestPathContainer::addShortestPathCollection(size_t startNodeIndex, uint startingInstant, size_t graphSize,
                                                             const TransitStateContainer& algorithmResultStates) {
    std::vector<TransitShortestPath> shortestPathList;
    shortestPathList.reserve(graphSize);

    //Convert states to shortest paths and add to collection
    for(size_t i = 0; i < graphSize; ++i) {
        shortestPathList.emplace_back(algorithmResultStates.getBestSolution(i));
    }

    //Add the (startingInstant, pathVector) pair at the appropriate node index
    container[startNodeIndex].emplace_back(startingInstant, shortestPathList);
}

std::pair<size_t, std::vector<TransitShortestPath>> *
TransitShortestPathContainer::getShortestPathsFrom(size_t startNodeIndex, uint earliestStartInstant) {
    const auto& iterator = std::ranges::lower_bound(container[startNodeIndex],
                                            std::pair<size_t , std::vector<TransitShortestPath>>(earliestStartInstant, {}));

    return iterator.base();
}

TransitShortestPath
TransitShortestPathContainer::getShortestPathToYFromTime(size_t startNodeIndex, uint earliestStartInstant, size_t goalNode) {
    auto [startNode, shortestPathsVector] = *getShortestPathsFrom(startNodeIndex, earliestStartInstant);
    for(const auto& shortestPath : shortestPathsVector) {
        if(shortestPath.getArrivalNode() == goalNode) {
            return shortestPath;
        }
    }
    return  TransitShortestPath{};
}
