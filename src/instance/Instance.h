//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_INSTANCE_H
#define GREEDYALGORITHM_INSTANCE_H
#include <vector>
#include "requests/Request.h"
#include "graph/Graph.h"

class Instance {
private:
    std::vector<Request> requests;
    Graph graph;

public:
    [[nodiscard]] Graph const & getGraph() const { return graph;}
    [[nodiscard]] std::vector<Request> const & getRequests() const { return requests;}
};


#endif //GREEDYALGORITHM_INSTANCE_H
