//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"

class SAEVRoute {
private:
    std::vector<SAEVKeyPoint> _route;
    SAEVehicle* _vehiclePointer{};
public:
    SAEVRoute() = default;
    explicit SAEVRoute(SAEVehicle* pointer) { _vehiclePointer = pointer; }

    /**
     * Moves an existing KeyPoint to the end of the SAEV _route.
     * Prefer using the two parameter version of this function to create objects in place.
     * @see emplace_back(Request*, bool)
     * @param keyPoint An existing Key Point to add to the SAEVKeyPoint vector
     */
    void push_back(SAEVKeyPoint& keyPoint) { _route.push_back(std::move(keyPoint)); }
    /**
     * Creates a new KeyPoint to the SAEV _route
     * @param request pointer to the request that will enter/leave the vehicle/_route on this key point
     * @param isEntry bool value  that's true if the request enters the vehicle and false if it's leaving the vehicle
     */
    void emplace_back(Request* request, bool  isEntry) { _route.emplace_back(request, isEntry); }

    [[nodiscard]] const std::vector<SAEVKeyPoint> &getRoute() const {
        return _route;
    }

    [[nodiscard]] SAEVehicle *getVehiclePointer() const {
        return _vehiclePointer;
    }

    void setVehiclePointer(SAEVehicle *vehiclePointer) {
        SAEVRoute::_vehiclePointer = vehiclePointer;
    }
};


#endif //GREEDYALGORITHM_SAEVROUTE_H
