//
// Created by romain on 18/06/24.
//

#ifndef GREEDYALGORITHM_BESTINSERTIONQUEUE_H
#define GREEDYALGORITHM_BESTINSERTIONQUEUE_H


#include <queue>
#include "BestRequestInsertion.h"

class BestInsertionQueue : public std::priority_queue<BestRequestInsertion, std::vector<BestRequestInsertion>, std::greater<>> {
private:
    SAEVKeyPoint &_originKP;
    size_t _vehicleId{std::numeric_limits<size_t>::max()};

public:
    explicit BestInsertionQueue(SAEVKeyPoint &originKP) : _originKP(originKP) {}

    BestInsertionQueue(SAEVKeyPoint &originKP, size_t vehicleId) : _originKP(originKP), _vehicleId(vehicleId) {}

    BestInsertionQueue(SAEVKeyPoint &originKP, size_t vehicleId, size_t nbRequests) : BestInsertionQueue(originKP, vehicleId) {
        c.reserve(nbRequests^2);
    }

    [[nodiscard]] size_t getVehicleId() const {
        return _vehicleId;
    }

    [[nodiscard]] SAEVKeyPoint &getOriginKp() const {
        return _originKP;
    }

    /**
     * Returns and removes the top value from the priority queue
     * @return The top value in the priority queue
     */
    BestRequestInsertion topAndPop() {
        BestRequestInsertion top = this->top();
        this->pop();
        return top;
    }
};


#endif //GREEDYALGORITHM_BESTINSERTIONQUEUE_H
