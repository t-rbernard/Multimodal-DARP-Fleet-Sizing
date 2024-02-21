//
// Created by Bebs on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TIMEDEPENDENTSHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_TIMEDEPENDENTSHORTESTPATHCONTAINER_H

#include <vector>
#include "ShortestPathContainer.h"

template <typename KeyPoint>
class TimeDependentShortestPathContainer {
public:
    virtual ShortestPath<KeyPoint> findShortestPath(int startNode, int destinationNode, int startTimestep) = 0;
    virtual std::vector<ShortestPath<KeyPoint>> findDestinationsFrom(int startNode, int startTimestep) = 0;
    virtual std::vector<ShortestPath<KeyPoint>> findDepartureListToArriveAtWindow(int destinationNode, int minArrivalTimestep, int maxArrivalTimestep) = 0;
};


#endif //GREEDYALGORITHM_TIMEDEPENDENTSHORTESTPATHCONTAINER_H
