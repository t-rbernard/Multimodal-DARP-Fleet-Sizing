//
// Created by romain on 22/03/24.
//

#include "SAEVehicle.h"

/**
 * Moves an existing KeyPoint to the end of the SAEV _route.
 * Prefer using the two parameter version of this function to create objects in place.
 * @see emplace_back(Request*, bool)
 * @param keyPoint An existing Key Point to add to the SAEVKeyPoint vector
 */
void SAEVehicle::push_back(const SAEVKeyPoint& keyPoint) { _route.push_back(keyPoint); }
/**
 * Creates a new KeyPoint to the SAEV _route
 * @param request pointer to the request that will enter/leave the vehicle/_route on this key point
 * @param isEntry bool value  that's true if the request enters the vehicle and false if it's leaving the vehicle
 * @see SAEVRoute::emplace_back
 */
void SAEVehicle::emplace_back(Request* request, bool  isEntry) { _route.emplace_back(request, isEntry); }
