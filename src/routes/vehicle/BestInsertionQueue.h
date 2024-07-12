//
// Created by romain on 18/06/24.
//

#ifndef GREEDYALGORITHM_BESTINSERTIONQUEUE_H
#define GREEDYALGORITHM_BESTINSERTIONQUEUE_H


#include <queue>
#include "BestRequestInsertion.h"

class BestInsertionQueue : public std::priority_queue<BestRequestInsertion, std::vector<BestRequestInsertion>, std::greater<>> {
private:
    size_t _requestId{std::numeric_limits<size_t>::max()};
    size_t _vehicleId{std::numeric_limits<size_t>::max()};

public:
    explicit BestInsertionQueue(size_t requestId) : _requestId(requestId) {}
    BestInsertionQueue(size_t requestId, size_t vehicleId) : _requestId(requestId), _vehicleId(vehicleId) {}

    BestInsertionQueue(size_t requestId, size_t vehicleId, size_t nbRequests) : BestInsertionQueue(requestId, vehicleId) {
        c.reserve(nbRequests^2);
    }

    [[nodiscard]] size_t getVehicleId() const {
        return _vehicleId;
    }

    [[nodiscard]] size_t getRequestId() const {
        return _requestId;
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
