//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVEHICLE_H
#define GREEDYALGORITHM_SAEVEHICLE_H

#define VEHICLE_CAPACITY 6


#include "../routes/SAEVRoute.h"

class SAEVehicle {
private:
    static const int _capacity{VEHICLE_CAPACITY};
    SAEVRoute _route{this}; //Associate the route with the vehicle via pointer

public:
    SAEVehicle() = default;
//    explicit SAEVehicle(const int capacity) : _capacity(capacity) {}

    [[nodiscard]] static int getCapacity() {
        return _capacity;
    }

    void push_back(SAEVKeyPoint& keyPoint);
    void emplace_back(Request* request, bool  isEntry);
};


#endif //GREEDYALGORITHM_SAEVEHICLE_H
