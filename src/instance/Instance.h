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

    std::vector<Request> _requests;
    Graph _graph;
    int _vehicleCapacity;

public:
    Instance(const std::string &graphFilePath, const std::string &requestsFilePath, const int vehicleCapacity);
    Instance(const std::vector<Request> &requests, Graph graph, const int vehicleCapacity) : _requests(
            requests), _graph(std::move(graph)), _vehicleCapacity(vehicleCapacity) {}

    [[nodiscard]] Graph const & getGraph() const { return _graph;}
    [[nodiscard]] std::vector<Request> const & getRequests() const { return _requests;}
    [[nodiscard]] int getVehicleCapacity() const { return _vehicleCapacity; }

    void parseRequestsFromFile(const std::string &basicString);
};


#endif //GREEDYALGORITHM_INSTANCE_H
