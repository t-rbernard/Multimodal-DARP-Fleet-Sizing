//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLESHORTESTPATH_H
#define GREEDYALGORITHM_VEHICLESHORTESTPATH_H

#include <compare>
#include "../ShortestPath.h"
#include "../../instance/graph/Node.h"
#include "../../utils/Globals.h"

class VehicleShortestPath : public ShortestPath<size_t> {
private:
    uint _duration{0};
public:
    VehicleShortestPath() = default;
    explicit VehicleShortestPath(uint duration) : _duration(duration) {}

    [[nodiscard]] uint getDuration() const {
        return _duration;
    }

    void setDuration(uint duration) {
        _duration = duration;
    }

    [[nodiscard]] size_t getDeparture() const override {
        return *_keyPoints.cbegin();
    }

    [[nodiscard]] size_t getArrival() const override {
        return *(_keyPoints.cend() - 1);
    }

    auto operator<=>(const VehicleShortestPath &rhs) const {
        if (this->getDeparture() != rhs.getDeparture() || this->getArrival() != rhs.getArrival()) {
            return std::partial_ordering::unordered;
        }

        return std::partial_order(_duration, rhs.getDuration());
    }
};


#endif //GREEDYALGORITHM_VEHICLESHORTESTPATH_H
