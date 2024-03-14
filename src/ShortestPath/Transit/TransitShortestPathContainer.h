//
// Created by romain on 13/03/24.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H


#include <vector>
#include "TransitShortestPath.h"

class TransitShortestPathContainer {
private:
    std::vector<std::pair<int, std::vector<TransitShortestPath>>> container; //Node vector -> Pair<Instant, ShortestPath[Node]>

public:
    void addShortestPathCollection(int startNodeIndex, std::pair<int, std::vector<TransitShortestPath>> shortestPathList);
    void addShortestPathCollection(int startNodeIndex, int startingInstant, std::vector<TransitAlgorithmState> algorithmResultStates);
    std::pair<int, std::vector<TransitShortestPath>> getShortestPathsFromTime(int startNodeIndex, int );

};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHCONTAINER_H
