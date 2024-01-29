//
// Created by romain on 22/01/24.
//

#ifndef GREEDYALGORITHM_INSTANCE_H
#define GREEDYALGORITHM_INSTANCE_H
#include <vector>
#include "requests/Request.h"
#include "graph/Graph.h"

class Instance {
    std::vector<Request> requests;
    Graph graph;
};


#endif //GREEDYALGORITHM_INSTANCE_H
