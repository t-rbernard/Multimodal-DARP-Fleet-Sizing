//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVEHICLE_H
#define GREEDYALGORITHM_SAEVEHICLE_H

#define VEHICLE_CAPACITY 6



class SAEVehicle {
private:
    static const int _capacity{VEHICLE_CAPACITY};

public:
    SAEVehicle() = default;
//    explicit SAEVehicle(const int capacity) : _capacity(capacity) {}

    [[nodiscard]] static int getCapacity() {
        return _capacity;
    }
};


#endif //GREEDYALGORITHM_SAEVEHICLE_H
