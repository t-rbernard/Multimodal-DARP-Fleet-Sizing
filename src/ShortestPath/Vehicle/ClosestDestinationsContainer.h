//
// Created by romain on 22/07/24.
//

#ifndef GREEDYALGORITHM_CLOSESTDESTINATIONSCONTAINER_H
#define GREEDYALGORITHM_CLOSESTDESTINATIONSCONTAINER_H


#include <cstddef>
#include <utility>
#include <queue>
#include <set>
#include "VehicleShortestPath.h"
#include "VehicleDestination.h"

class ClosestDestinationsContainer {
private:
    size_t _startingNodeIdx;
    std::set<VehicleDestination> _orderedDestinations{};

public:
    explicit ClosestDestinationsContainer(size_t startingNodeIdx) : _startingNodeIdx(startingNodeIdx) {}

    size_t getStartingNodeIdx() const {
        return _startingNodeIdx;
    }

    void setStartingNodeIdx(size_t startingNodeIdx) {
        _startingNodeIdx = startingNodeIdx;
    }

    [[nodiscard]] const std::set<VehicleDestination> &getOrderedDestinations() const {
        return _orderedDestinations;
    }

    auto cbegin() {
        return _orderedDestinations.cbegin();
    }

    void addDestination(size_t destinationNodeIdx, uint distance) {
        _orderedDestinations.emplace(destinationNodeIdx, distance);
    }
};


#endif //GREEDYALGORITHM_CLOSESTDESTINATIONSCONTAINER_H
