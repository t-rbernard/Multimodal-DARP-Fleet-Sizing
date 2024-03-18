//
// Created by romain on 13/03/24.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H


#include <vector>
#include "TransitShortestPath.h"

class TransitShortestPathContainer {
private:
    std::vector<std::vector<std::pair<int, std::vector<TransitShortestPath>>>> container; //NodeVector< PairVector<Pair<Instant, NodeVector<ShortestPath> >> >

public:
    void addShortestPathCollection(int startNodeIndex, const std::pair<int, std::vector<TransitShortestPath>>& shortestPathList);
    void addShortestPathCollection(int startNodeIndex, int startingInstant, int graphSize, const std::vector<TransitAlgorithmState>& algorithmResultStates);
    std::vector<std::pair<int, std::vector<TransitShortestPath>>>::iterator getShortestPathsFromTime(int startNodeIndex, int earliestStartInstant);
    std::pair<int, TransitShortestPath> getShortestPathToYFromTime(int startNodeIndex, int earliestStartInstant, int goalNode);

};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
