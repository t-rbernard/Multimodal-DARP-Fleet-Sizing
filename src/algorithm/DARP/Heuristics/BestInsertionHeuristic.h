//
// Created by romain on 20/06/24.
//

#ifndef GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H
#define GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H


#include <cstdlib>
#include "../../../routes/vehicle/SAEVRoute.h"

class BestInsertionHeuristic {
public:
    /**
     * Automatically inserts the given request in a vehicle, potentially creating a new one if no active vehicle works
     * @param requestId ID of the request to insert in the route
     * @param route the route structure in which the request will be inserted
     * @return ID of the vehicle in which the request has been
     */
    static size_t doBestRequestInsertionForRoute(size_t requestId, SAEVRoute& route);
    /**
     * Iteratively attempts insertions in the currently active vehicles in two steps :
     * 1) creates a global best insertion list for all active vehicles
     * 2) tries every insertions from best to worst
     * 3) return the ID of the vehicle in which the request was inserted if an active vehicle was viable, return the most favourable inactive vehicle's ID otherwise
     * @param requestId ID of the request to insert in the route
     * @param route the route structure in which the request will be inserted
     * @return The ID of the active vehicle in which our request was inserted, or the ID of the most favourable inactive vehicle
     */
    static SAEVRouteChangelist tryBestRequestInsertionInActiveVehicle(size_t requestId, SAEVRoute& route);
    /**
     * Iteratively tests best insertions wrt scoring function (detour) in the given vehicle and route
     * @param requestId ID of the request to insert in the vehicle
     * @param requestId ID of the vehicle in which to insert the vehicle
     * @param route
     * @return true iff the request was inserted in the vehicle, false if no best insertion yielded a possible insertion
     */
    static SAEVRouteChangelist tryVehicleBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route);
    /**
     * Tries the best insertions
     * @param bestInsertionsQueue
     * @param route
     * @return
     */
    static SAEVRouteChangelist tryVehicleBestInsertion(BestInsertionQueue& bestInsertionsQueue, SAEVRoute& route);


    /** TODO Implement those to prevent trying every single best insertion
    static bool vehicle_K_BestInsertion(size_t requestId, size_t vehicleId, SAEVRoute route);
    static bool vehicleScoreThresholdBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route); */
};


#endif //GREEDYALGORITHM_BESTINSERTIONHEURISTIC_H
