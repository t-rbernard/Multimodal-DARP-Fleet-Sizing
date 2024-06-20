//
// Created by romain on 20/06/24.
//

#ifndef GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H
#define GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H


#include <cstdlib>
#include "../../../routes/vehicle/SAEVRoute.h"

class BestInsertionHeuristic {
    /**
     * Automatically inserts the given request in a vehicle, potentially creating a new one if no active vehicle works
     * @param requestId ID of the request to insert in the route
     * @param route the route structure in which the request will be inserted
     * @return ID of the vehicle in which the request has been
     */
    static size_t doRouteBestInsertion(size_t requestId, SAEVRoute route);
    /**
     * Iteratively tests best insertions wrt scoring function (detour) in the given vehicle and route
     * @param requestId ID of the request to insert in the vehicle
     * @param requestId ID of the vehicle in which to insert the vehicle
     * @param route
     * @return true iff the request was inserted in the vehicle, false if no best insertion yielded a possible insertion
     */
    static bool tryVehicleBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route);

    /** TODO Implement those to prevent trying every single best insertion
    static bool vehicle_K_BestInsertion(size_t requestId, size_t vehicleId, SAEVRoute route);
    static bool vehicleScoreThresholdBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route); */
};


#endif //GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H
