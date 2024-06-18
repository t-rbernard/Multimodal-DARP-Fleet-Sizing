//
// Created by romain on 18/06/24.
//

#ifndef GREEDYALGORITHM_BESTINSERTIONQUEUE_H
#define GREEDYALGORITHM_BESTINSERTIONQUEUE_H


#include <queue>
#include "BestRequestInsertion.h"

class BestInsertionQueue : public std::priority_queue<BestRequestInsertion, std::vector<BestRequestInsertion>> {
private:
    size_t _requestId{std::numeric_limits<size_t>::max()};
    size_t _vehicleId{std::numeric_limits<size_t>::max()};

public:
    BestInsertionQueue(size_t requestId, size_t vehicleId) : _requestId(requestId), _vehicleId(vehicleId) {}

    BestInsertionQueue(size_t requestId, size_t vehicleId, size_t nbRequests) : BestInsertionQueue(requestId, vehicleId) {
        c.reserve(nbRequests^2);
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
