//
// Created by Bebs on 24/07/2024.
//

#ifndef GREEDYALGORITHM_TRANSITACCESS_H
#define GREEDYALGORITHM_TRANSITACCESS_H


#include <cstddef>
#include "../../../utils/Globals.h"

class TransitAccess {
    size_t _accessNodeIdx{};
    uint _accessTimestamp{};

public:
    TransitAccess() = default;
    TransitAccess(size_t accessNodeIdx, uint accessTimestamp) : _accessNodeIdx(accessNodeIdx),
                                                                _accessTimestamp(accessTimestamp) {}

    [[nodiscard]] size_t getAccessNodeIdx() const {
        return _accessNodeIdx;
    }

    void setAccessNodeIdx(size_t accessNodeIdx) {
        _accessNodeIdx = accessNodeIdx;
    }

    [[nodiscard]] uint getAccessTimestamp() const {
        return _accessTimestamp;
    }

    void setAccessTimestamp(uint accessTimestamp) {
        _accessTimestamp = accessTimestamp;
    }
};


#endif //GREEDYALGORITHM_TRANSITACCESS_H
