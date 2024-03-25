//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVEHICLE_H
#define GREEDYALGORITHM_SAEVEHICLE_H


#include "../routes/SAEVRoute.h"

class SAEVehicle {
private:
    const int _capacity;
    SAEVRoute _route;

public:
    explicit SAEVehicle(const int capacity) : _capacity(capacity) {}

    [[nodiscard]] int getCapacity() const {
        return _capacity;
    }

    void push_back(const SAEVKeyPoint& keyPoint);
    void emplace_back(Request* request, bool  isEntry);
};


#endif //GREEDYALGORITHM_SAEVEHICLE_H
