//
// Created by romain on 22/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLEDESTINATION_H
#define GREEDYALGORITHM_VEHICLEDESTINATION_H


#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <compare>

class VehicleDestination {
private:
    size_t _destinationNodeIdx;
    uint _distance{INT32_MAX};

public:
    explicit VehicleDestination(size_t destinationNodeIdx) : _destinationNodeIdx(destinationNodeIdx) {}
    VehicleDestination(size_t destinationNodeIdx, uint distance) : _destinationNodeIdx(destinationNodeIdx),
                                                                   _distance(distance) {}

    [[nodiscard]] size_t getDestinationNodeIdx() const {
        return _destinationNodeIdx;
    }

    void setDestinationNodeIdx(size_t destinationNodeIdx) {
        _destinationNodeIdx = destinationNodeIdx;
    }

    [[nodiscard]] uint getDistance() const {
        return _distance;
    }

    void setDistance(uint distance) {
        _distance = distance;
    }

    auto operator<=>(const VehicleDestination &rhs) const {
        return std::strong_order(_distance, rhs.getDistance());
    }
};


#endif //GREEDYALGORITHM_VEHICLEDESTINATION_H
