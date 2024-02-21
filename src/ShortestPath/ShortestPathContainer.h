//
// Created by Bebs on 20/02/2024.
//

#ifndef GREEDYALGORITHM_SHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_SHORTESTPATHCONTAINER_H

#include "ShortestPath.h"

/**
 * Abstract class defining the search API for x to y shortest path containers
 */
template <typename KeyPoint>
class ShortestPathContainer {
public:
    virtual ShortestPath<KeyPoint> findShortestPath(int startNode, int destinationNode, int startTimestep) = 0;
};


#endif //GREEDYALGORITHM_SHORTESTPATHCONTAINER_H
