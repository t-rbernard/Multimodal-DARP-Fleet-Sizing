//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_INSTANCE_H
#define GREEDYALGORITHM_INSTANCE_H
#include <utility>
#include <vector>
#include "requests/Request.h"
#include "graph/Graph.h"
#include "SAEVehicle.h"

class Instance {
private:
    std::vector<Request> requests;
    Graph graph;
    int vehicleCapacity;

public:
    Instance(const std::vector<Request> &requests, Graph graph, const int vehicleCapacity) : requests(
            requests), graph(std::move(graph)), vehicleCapacity(vehicleCapacity) {}

    [[nodiscard]] Graph const & getGraph() const { return graph;}
    [[nodiscard]] std::vector<Request> const & getRequests() const { return requests;}
    [[nodiscard]] int getVehicleCapacity() const { return vehicleCapacity; }
};


#endif //GREEDYALGORITHM_INSTANCE_H
