//
// Created by romain on 13/03/24.
//

#include "TransitShortestPathContainer.h"
#include "TransitStateContainer.h"

void TransitShortestPathContainer::addShortestPathCollection(int startNodeIndex,
                                                             const std::pair<int, std::vector<TransitShortestPath>>& shortestPathList) {
    container.at(startNodeIndex).emplace_back(shortestPathList);
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
    container.at(startNodeIndex).emplace_back(startingInstant, shortestPathList);
}

std::vector<std::pair<int, std::vector<TransitShortestPath>>>::iterator
TransitShortestPathContainer::getShortestPathsFromTime(int startNodeIndex, int earliestStartInstant) {
    const auto& iterator = std::lower_bound(container.at(startNodeIndex).begin(), container.at(startNodeIndex).end(),
                                            std::pair<int, std::vector<TransitShortestPath>>(earliestStartInstant, {}));

    return iterator;
}

std::pair<int, TransitShortestPath>
TransitShortestPathContainer::getShortestPathToYFromTime(int startNodeIndex, int earliestStartInstant, int goalNode) {
    const auto& shortestPathsIterator = getShortestPathsFromTime(startNodeIndex, earliestStartInstant);
    return std::pair(shortestPathsIterator->first, shortestPathsIterator->second.at(goalNode));
}
