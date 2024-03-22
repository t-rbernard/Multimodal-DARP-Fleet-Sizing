//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVEHICLE_H
#define GREEDYALGORITHM_SAEVEHICLE_H


class SAEVehicle {
private:
    const int capacity;

public:
    explicit SAEVehicle(const int capacity) : capacity(capacity) {}

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }
};


#endif //GREEDYALGORITHM_SAEVEHICLE_H
