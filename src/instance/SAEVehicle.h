//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVEHICLE_H
#define GREEDYALGORITHM_SAEVEHICLE_H

#include "../utils/Constants.h"

class SAEVehicle {
private:
    static const int _capacity{Constants::VEHICLE_CAPACITY};

public:
    SAEVehicle() = default;
//    explicit SAEVehicle(const int capacity) : _capacity(capacity) {}

    [[nodiscard]] static int getCapacity() {
        return _capacity;
    }
};


#endif //GREEDYALGORITHM_SAEVEHICLE_H
