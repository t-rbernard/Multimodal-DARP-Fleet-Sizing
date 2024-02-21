//
// Created by Bebs on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H

#include "../TimeDependentShortestPathContainer.h"
#include "TransitShortestPath.h"

class TransitShortestPathPrecompute {
    static ShortestPathContainer<TransitShortestPath> formatResults(TimeDependentShortestPathContainer<TransitShortestPath> container);
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATHPRECOMPUTE_H
