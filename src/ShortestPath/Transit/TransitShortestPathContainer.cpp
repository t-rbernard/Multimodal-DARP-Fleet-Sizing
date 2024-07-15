//
// Created by romain on 13/03/24.
//

#include <algorithm>
#include "TransitShortestPathContainer.h"
#include "TransitStateContainer.h"

void TransitShortestPathContainer::addShortestPathCollection(int startNodeIndex,
                                                             const std::pair<int, std::vector<TransitShortestPath>>& shortestPathList) {
    container[startNodeIndex].emplace_back(shortestPathList);
}

void TransitShortestPathContainer::addShortestPathCollection(int startNodeIndex, int startingInstant, int graphSize,
                                                             const TransitStateContainer& algorithmResultStates) {
    std::vector<TransitShortestPath> shortestPathList;
    shortestPathList.reserve(graphSize);

    //Convert states to shortest paths and add to collection
    for(int i = 0; i < graphSize; ++i) {
        shortestPathList.emplace_back(algorithmResultStates.getBestSolution(i));
    }

    //Add the (startingInstant, pathVector) pair at the appropriate node index
    container[startNodeIndex].emplace_back(startingInstant, shortestPathList);
}

std::pair<int, std::vector<TransitShortestPath>> *
TransitShortestPathContainer::getShortestPathsFrom(int startNodeIndex, int earliestStartInstant) {
    const auto& iterator = std::ranges::lower_bound(container[startNodeIndex],
                                            std::pair<int, std::vector<TransitShortestPath>>(earliestStartInstant, {}));

    return iterator.base();
}

TransitShortestPath
TransitShortestPathContainer::getShortestPathToYFromTime(int startNodeIndex, int earliestStartInstant, int goalNode) {
    auto [startNode, shortestPathsVector] = *getShortestPathsFrom(startNodeIndex, earliestStartInstant);
    for(const auto& shortestPath : shortestPathsVector) {
        if(shortestPath.getArrivalNode() == goalNode) {
            return shortestPath;
        }
    }
    return  TransitShortestPath{};
}
