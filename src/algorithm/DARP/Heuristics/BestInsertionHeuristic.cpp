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

SAEVRouteChangelist BestInsertionHeuristic::tryVehicleBestInsertion(SAEVKeyPoint &requestKp, size_t vehicleId, SAEVRoute& route) {
    BestInsertionQueue bestInsertionsQueue = route.getBestFeasibleInsertionsQueue(requestKp, vehicleId);
    return tryVehicleBestInsertion(bestInsertionsQueue, route);
}

SAEVRouteChangelist BestInsertionHeuristic::tryVehicleBestInsertion(BestInsertionQueue& bestInsertionsQueue, SAEVRoute& route) {
    bool bestInsertionFound = false;
    BestRequestInsertion currentBestInsertion;
    SAEVKeyPoint & requestKp = bestInsertionsQueue.getOriginKp();
    SAEVRouteChangelist lastInsertionChangelist(&route, &requestKp);

    while(!bestInsertionsQueue.empty() && !bestInsertionFound) {
        currentBestInsertion = bestInsertionsQueue.topAndPop();
        DEBUG_MSG("Trying insertion " + currentBestInsertion.to_string() + ", remaining : " + std::to_string(bestInsertionsQueue.size()));
        lastInsertionChangelist = route.tryAddRequest(requestKp,currentBestInsertion.getOriginInsertionKp(),currentBestInsertion.getDestinationInsertionKp());
        //If insertion worked, signal it, otherwise revert changes
        if(lastInsertionChangelist.getStatus() == SAEVRouteChangelist::InsertionStatus::SUCCESS) {
            DEBUG_MSG("\tBest valid insertion found !\n\t\t" + currentBestInsertion.to_string());
            bestInsertionFound = true;
        } else {
            DEBUG_MSG("Failed to insert best insertion " + currentBestInsertion.to_string());
            lastInsertionChangelist.revertChanges();
        }
    }

    return lastInsertionChangelist;
}

size_t BestInsertionHeuristic::doBestRequestInsertionForRoute(SAEVKeyPoint &requestKp, SAEVRoute& route) {
    size_t vehicleId = 0;
    bool insertionSuccess{false};
    //Iteratively try inserting in every active vehicle and the first inactive vehicle
    do {
        insertionSuccess = tryVehicleBestInsertion(requestKp, vehicleId, route).success();
    } while(!insertionSuccess && ++vehicleId <= route.getLastActiveVehicleId() + 1);

    //Update last active vehicle ID
    if(vehicleId > route.getLastActiveVehicleId()) {
        route.insertRequestInNewVehicle(requestKp);
    }

    return vehicleId;
}

SAEVRouteChangelist BestInsertionHeuristic::tryBestRequestInsertionInActiveVehicle(SAEVKeyPoint &requestKp, SAEVRoute &route) {
    size_t vehicleId = 0;
    BestInsertionQueue bestInsertions{requestKp};
    //Iteratively try inserting in every active vehicle and the first inactive vehicle
    do {
        route.getBestFeasibleInsertionsQueue(bestInsertions, requestKp, vehicleId);
    } while(++vehicleId <= route.getLastActiveVehicleId());

    return tryVehicleBestInsertion(bestInsertions, route);
}
