//
// Created by romain on 20/06/24.
//

#include "BestInsertionHeuristic.h"

#ifdef DEBUG_TRANSIT_PRECOMPUTE
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

bool BestInsertionHeuristic::tryVehicleBestInsertion(size_t requestId, size_t vehicleId, SAEVRoute& route) {
    BestInsertionQueue bestInsertionsQueue = route.getBestFeasibleInsertionsQueue(requestId, vehicleId); //TODO: check perfs between BestInsertionsQueue vs BestFeasibleInsertionsQueue
    bool bestInsertionFound = false;
    BestRequestInsertion currentBestInsertion;

    while(!bestInsertionsQueue.empty() && !bestInsertionFound) {
        currentBestInsertion = bestInsertionsQueue.topAndPop();
        SAEVRouteChangelist lastInsertionChangelist = route.tryAddRequest(requestId,
                                                                          *currentBestInsertion.getOriginInsertionKp(),
                                                                          *currentBestInsertion.getDestinationInsertionKp());
        //If insertion worked, signal it, otherwise revert changes
        if(lastInsertionChangelist.getStatus() == SAEVRouteChangelist::InsertionStatus::SUCCESS) {
            DEBUG_MSG("Best valid insertion found !\n\t" + currentBestInsertion.to_string());
            bestInsertionFound = true;
        } else {
            lastInsertionChangelist.revertChanges();
        }
    }
    return bestInsertionFound;
}

size_t BestInsertionHeuristic::doRouteBestInsertion(size_t requestId, SAEVRoute route) {
    size_t vehicleId = 0;
    bool insertionSuccess{false};
    while(vehicleId <= route.getLastActiveVehicleId() + 1 && !insertionSuccess) {
        insertionSuccess = tryVehicleBestInsertion(requestId, vehicleId, route);
    }
    return vehicleId;
}
