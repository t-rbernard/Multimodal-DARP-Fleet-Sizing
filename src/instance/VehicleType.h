//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_VEHICLETYPE_H
#define GREEDYALGORITHM_VEHICLETYPE_H


class VehicleType {
private:
    const int capacity;

public:
    explicit VehicleType(const int capacity) : capacity(capacity) {}

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }
};


#endif //GREEDYALGORITHM_VEHICLETYPE_H
