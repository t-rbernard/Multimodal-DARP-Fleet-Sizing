//
// Created by romain on 13/03/24.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H


#include <vector>
#include "TransitShortestPath.h"
#include "TransitStateContainer.h"

class TransitShortestPathContainer {
private:
    std::vector<std::vector<std::pair<size_t, std::vector<TransitShortestPath>>>> container; //NodeVector< PairVector<Pair<Instant, NodeVector<ShortestPath> >> >

public:
    explicit TransitShortestPathContainer(int size) { container.resize(size); }
    explicit TransitShortestPathContainer(size_t size) { container.resize(size); }
    void addShortestPathCollection(size_t startNodeIndex, const std::pair<size_t , std::vector<TransitShortestPath>>& shortestPathList);
    void addShortestPathCollection(size_t startNodeIndex, uint startingInstant, size_t graphSize, const TransitStateContainer& algorithmResultStates);
    std::pair<size_t, std::vector<TransitShortestPath>>& getShortestPathsFrom(size_t startNodeIndex, uint earliestStartInstant);
    TransitShortestPath getShortestPathToYFromTime(size_t startNodeIndex, uint earliestStartInstant, size_t goalNode);
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
