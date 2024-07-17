//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLEPATHSTATE_H
#define GREEDYALGORITHM_VEHICLEPATHSTATE_H

#include <limits>
#include <cstdlib>
#include <compare>

class VehiclePathState {
private:
    size_t _nodeIndex{std::numeric_limits<size_t>::max()};
    uint _instant{std::numeric_limits<uint>::max()};

public:
    VehiclePathState() = default;
    VehiclePathState(size_t nodeIndex, uint instant) : _nodeIndex(nodeIndex), _instant(instant) {}

    [[nodiscard]] size_t getNodeIndex() const {
        return _nodeIndex;
    }

    void setNodeIndex(size_t nodeIndex) {
        _nodeIndex = nodeIndex;
    }

    [[nodiscard]] uint getInstant() const {
        return _instant;
    }

    void setInstant(uint instant) {
        _instant = instant;
    }

    auto operator<=>(const VehiclePathState &rhs) const {
        if (this->_nodeIndex != rhs.getNodeIndex()) {
            return std::partial_ordering::unordered;
        }

        return std::partial_order(_instant, rhs.getInstant());
    }
};


#endif //GREEDYALGORITHM_VEHICLEPATHSTATE_H
