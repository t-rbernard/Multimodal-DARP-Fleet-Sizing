//
// Created by romain on 20/06/24.
//

#include "BestInsertionHeuristic.h"

#ifdef DEBUG_BEST_INSERTION_HEURISTIC
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

bool BestInsertionHeuristic::tryVehicleBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route) {
    BestInsertionQueue bestInsertionsQueue = route.getBestFeasibleInsertionsQueue(requestId, vehicleId); //TODO: check perfs between BestInsertionsQueue vs BestFeasibleInsertionsQueue
    bool bestInsertionFound = false;
    BestRequestInsertion currentBestInsertion;

    DEBUG_MSG("Trying to insert request " + std::to_string(requestId) + " in vehicle " + std::to_string(vehicleId) + " queue size : " + std::to_string(bestInsertionsQueue.size()));
    while(!bestInsertionsQueue.empty() && !bestInsertionFound) {
        currentBestInsertion = bestInsertionsQueue.topAndPop();
        DEBUG_MSG("Trying insertion " + currentBestInsertion.to_string() + ", remaining : " + std::to_string(bestInsertionsQueue.size()));
        SAEVRouteChangelist lastInsertionChangelist = route.tryAddRequest(requestId,
                                                                          *currentBestInsertion.getOriginInsertionKp(),
                                                                          *currentBestInsertion.getDestinationInsertionKp());
        //If insertion worked, signal it, otherwise revert changes
        if(lastInsertionChangelist.getStatus() == SAEVRouteChangelist::InsertionStatus::SUCCESS) {
            DEBUG_MSG("\tBest valid insertion found !\n\t\t" + currentBestInsertion.to_string());
            bestInsertionFound = true;
        } else {
            DEBUG_MSG("Failed to insert best insertion " + currentBestInsertion.to_string());
            lastInsertionChangelist.revertChanges();
        }
    }
    return bestInsertionFound;
}

size_t BestInsertionHeuristic::doBestRequestInsertionForRoute(size_t requestId, SAEVRoute& route) {
    size_t vehicleId = 0;
    bool insertionSuccess{false};
    //Iteratively try inserting in every active vehicle and the first inactive vehicle
    do {
        insertionSuccess = tryVehicleBestInsertion(requestId, vehicleId, route);
    } while(!insertionSuccess && ++vehicleId <= route.getLastActiveVehicleId() + 1);

    //Update last active vehicle ID
    if(vehicleId > route.getLastActiveVehicleId()) {
        route.setLastActiveVehicleId(vehicleId);
        DEBUG_MSG("NEW VEHICLE CREATED, ID :" + std::to_string(vehicleId));
    }

    return vehicleId;
}

size_t BestInsertionHeuristic::tryBestRequestInsertionInActiveVehicle(size_t requestId, SAEVRoute &route) {
    size_t vehicleId = 0;
    bool insertionSuccess{false};
    //Iteratively try inserting in every active vehicle and the first inactive vehicle
    while(!insertionSuccess && ++vehicleId <= route.getLastActiveVehicleId() + 1) {
        insertionSuccess = tryVehicleBestInsertion(requestId, vehicleId, route);
    }

    if(insertionSuccess)
        return vehicleId;
    else
        return route.getLastActiveVehicleId() + 1;
}
