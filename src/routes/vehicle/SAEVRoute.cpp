//
// Created by romain on 22/03/24.
//

#include <queue>
#include <iostream>
#include "SAEVRoute.h"
#include "BestInsertionQueue.h"

// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
#define assertm(exp, msg) assert(((void)msg, exp))

#ifdef DEBUG_CONSTRAINT_PROPAGATION
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

SAEVRoute::SAEVRoute(const Graph &graph, const std::vector<Request> &requestList)
        : SAEVRoute(graph, requestList, false) {}

SAEVRoute::SAEVRoute(const Graph &graph, const std::vector<Request> &requestList, bool initMultimodal)
        : _nbRequest(requestList.size()), _graph(&graph), _requestList(&requestList) {

    if(!initMultimodal) {
        _route.resize(_nbRequest*4); //nbRequest*2 O/D KeyPoints + nbRequest*2 Start/End depots (upper bound #vehicle = #requests
    } else {
        _route.resize(_nbRequest * 8); // 2*NbRequests O/D Keypoints, 2*NbRequests O/D vehicle depot Keypoints
                                                // 2*NbRequests O/D Bus entry Keypoints, 2*NbRequests O/D Bus exit Keypoints
    }

    //Init Request O/D and Depot start/end key points
    for(size_t i = 0; i < _nbRequest; ++i) {
        //Create key O/D points
        getRequestOrigin(i) = SAEVKeyPoint(graph, requestList[i], true); //origin
        getRequestDestination(i) = SAEVKeyPoint(graph, requestList[i], false); //destination
        //Link Origins and Destinations
        getRequestOrigin(i).setCounterpart(&getRequestDestination(i));
        getRequestDestination(i).setCounterpart(&getRequestOrigin(i));

        //Create depot O/D KP (Upper Bound = nb requests)
        getOriginDepot(i) = SAEVKeyPoint(graph.getDepotNodeIdx()); //start
        getDestinationDepot(i) = SAEVKeyPoint(graph.getDepotNodeIdx()); //end
        //Link Origin depots and Destination depots
        getOriginDepot(i).setCounterpart(&getDestinationDepot(i));
        getDestinationDepot(i).setCounterpart(&getOriginDepot(i));
        getOriginDepot(i).setSuccessor(&getDestinationDepot(i));
        getDestinationDepot(i).setPredecessor(&getOriginDepot(i));
    }

    //Set values and init all links for multimodal keypoints
    if(initMultimodal) {
        for (size_t i = 0; i < _nbRequest; ++i) {
            //Set isOrigin accordingly
            getEntrySubRequestOrigin(i).setIsOrigin(true);
            getExitSubRequestOrigin(i).setIsOrigin(true);

            //Link Origins and Destinations for entry/exit subrequests
            getEntrySubRequestOrigin(i).setCounterpart(&getEntrySubRequestDestination(i));
            getEntrySubRequestDestination(i).setCounterpart(&getEntrySubRequestOrigin(i));
            getExitSubRequestOrigin(i).setCounterpart(&getExitSubRequestDestination(i));
            getExitSubRequestDestination(i).setCounterpart(&getExitSubRequestOrigin(i));
        }
    }
}

void SAEVRoute::insertRequest(SAEVKeyPoint &originKp, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP) {
    SAEVKeyPoint* destinationKp = originKp.getCounterpart();

    SAEVKeyPoint *originSuccKp = originRequestPredecessorKP->getSuccessor();
    SAEVKeyPoint *destinationSuccKp = destinationRequestPredecessorKP->getSuccessor();

    if(originRequestPredecessorKP != destinationRequestPredecessorKP) {
        //Set values for O/D KPs
        originKp.setPredecessor(originRequestPredecessorKP);
        originKp.setSuccessor(originSuccKp);
        destinationKp->setPredecessor(destinationRequestPredecessorKP);
        destinationKp->setSuccessor(destinationSuccKp);

        //Set values for predecessors/successors
        originRequestPredecessorKP->setSuccessor(&originKp);
        originSuccKp->setPredecessor(&originKp);
        destinationRequestPredecessorKP->setSuccessor(destinationKp);
        destinationSuccKp->setPredecessor(destinationKp);
    } else {
        //Set values for O/D KPs
        originKp.setPredecessor(originRequestPredecessorKP);
        originKp.setSuccessor(destinationKp);
        destinationKp->setPredecessor(&originKp);
        destinationKp->setSuccessor(destinationSuccKp);

        //Set values for predecessors/successors
        originRequestPredecessorKP->setSuccessor(&originKp);
        originSuccKp->setPredecessor(destinationKp);
    }
    //Once insertion is done, update weights on the route
    addRequestWeightToRoute(originKp);
}

void SAEVRoute::removeRequest(SAEVKeyPoint &originKp) {
    assertm(originKp.getSuccessor() != nullptr, "Trying to remove a request that was already removed");
    //Before undoing the insertion, update weights on the route
    removeRequestWeightFromRoute(originKp);

    SAEVKeyPoint* destinationKp = originKp.getCounterpart();

    //get predecessor and successor for request
    SAEVKeyPoint* originPredecessor = originKp.getPredecessor();
    SAEVKeyPoint* originSuccessor = originKp.getSuccessor();
    SAEVKeyPoint* destinationPredecessor = destinationKp->getPredecessor();
    SAEVKeyPoint* destinationSuccessor = destinationKp->getSuccessor();

    //Link pred and successor from origin and destination (cases differ if O/D are next to each other
    if(originSuccessor == destinationKp) {
        originPredecessor->setSuccessor(destinationSuccessor);
        destinationSuccessor->setPredecessor(originPredecessor);
    } else {
        originPredecessor->setSuccessor(originSuccessor);
        originSuccessor->setPredecessor(originPredecessor);
        destinationPredecessor->setSuccessor(destinationSuccessor);
        destinationSuccessor->setSuccessor(destinationPredecessor);
    }

    //Revert origin/destination key points to their default state
    originKp.setPredecessor(nullptr);
    originKp.setSuccessor(nullptr);
    destinationKp->setPredecessor(nullptr);
    destinationKp->setSuccessor(nullptr);
    originKp.setMinTw(originKp.getRequest()->getMinDepartureTw());
    originKp.setMaxTw(originKp.getRequest()->getMaxDepartureTw());
    destinationKp->setMinTw(originKp.getRequest()->getMinArrivalTw());
    destinationKp->setMaxTw(originKp.getRequest()->getMaxArrivalTw());
}

SAEVRouteChangelist
SAEVRoute::tryAddRequest(size_t requestId, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP) {
    return tryAddRequest(getRequestOrigin(requestId), originRequestPredecessorKP, destinationRequestPredecessorKP);
}

SAEVRouteChangelist
SAEVRoute::tryAddRequest(SAEVKeyPoint &requestOriginKeyPoint, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP) {
    const Request* request = requestOriginKeyPoint.getRequest();
    SAEVKeyPoint const* destinationSuccessor = destinationRequestPredecessorKP->getSuccessor();

    //Check vehicle capacity
    SAEVKeyPoint const* currentKP = originRequestPredecessorKP;
    do {
        if(currentKP->getCurrentOccupation() + request->getWeight() > SAEVehicle::getCapacity()) {
            DEBUG_MSG("WEIGHT VIOLATION : request weight = " + std::to_string(request->getWeight()) + " incompatible KP = " + currentKP->to_string());
            return SAEVRouteChangelist(this, &requestOriginKeyPoint, originRequestPredecessorKP, destinationRequestPredecessorKP, SAEVRouteChangelist::InsertionStatus::FAILURE_PRECONDITION_WEIGHT);
        }
        currentKP = currentKP->getSuccessor();
    } while (currentKP != destinationSuccessor && currentKP != nullptr);

    //Do basic checks on neighbouring nodes from our Origin/Destination insertion points
    bool isValid = doNeighbouringTWChecks(requestOriginKeyPoint, originRequestPredecessorKP, destinationRequestPredecessorKP);

    if(isValid) {
        return insertRequestWithPropagation(requestOriginKeyPoint, originRequestPredecessorKP, destinationRequestPredecessorKP);
    } else {
        DEBUG_MSG("TW VIOLATION on neighbour KPs");
        return SAEVRouteChangelist(this, &requestOriginKeyPoint, originRequestPredecessorKP, destinationRequestPredecessorKP, SAEVRouteChangelist::InsertionStatus::FAILURE_PRECONDITION_TW);
    }
}

bool
SAEVRoute::doNeighbouringTWChecks(const SAEVKeyPoint &originKP, const SAEVKeyPoint *originPredecessor, const SAEVKeyPoint *destinationPredecessor) const {

    const SAEVKeyPoint* destinationKP = originKP.getCounterpart();
    const SAEVKeyPoint* originSuccessor = originPredecessor->getSuccessor();
    const size_t originNodeIndex = originKP.getNodeIndex();
    const size_t destinationNodeIndex = destinationKP->getNodeIndex();

    if(originPredecessor != destinationPredecessor)
    {
        SAEVKeyPoint const* destinationSuccessor = destinationPredecessor->getSuccessor();

        //Tests time windows Origin (yes this if-else could be one giant OR, but I'd rather separate every case)
        uint predOriginTimeWindow = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), originNodeIndex);
        uint predDestinationTimeWindow = destinationPredecessor->getMinTw() + _graph->getShortestSAEVPath(destinationPredecessor->getNodeIndex(), destinationNodeIndex);
        if(predOriginTimeWindow > originKP.getMaxTw())
            return false;
        else if(originKP.getMinTw() + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) // Could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
        //Tests time windows Destination
        else if(predDestinationTimeWindow > destinationKP->getMaxTw())
            return false;
        else if(destinationKP->getMinTw() + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw()) //could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw())
            return false;
    } else { //We need a specific case if origin and destination are inserted after the same node
        uint predMinTWToOrigin = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), originNodeIndex);
        uint predMinTWToDest = predMinTWToOrigin + _graph->getShortestSAEVPath(originNodeIndex, destinationNodeIndex);
        if(predMinTWToOrigin > originKP.getMaxTw()) //Path from pred to O
            return false;
        else if(predMinTWToDest > destinationKP->getMaxTw()) //Path from pred to D
            return false;
        else if(predMinTWToDest + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from pred to successor
            return false;
        else if(destinationKP->getMinTw() + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from D to successor
            return false;
        else if(originKP.getMinTw() + _graph->getShortestSAEVPath(originNodeIndex, destinationNodeIndex) //Path from O to successor
        + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
    }

    return true;
}

SAEVRouteChangelist SAEVRoute::insertRequestWithPropagation(SAEVKeyPoint &originKP, SAEVKeyPoint * originRequestPredecessorKP,
                                                            SAEVKeyPoint * destinationRequestPredecessorKP) {
    //Init changelist and detour score
    SAEVRouteChangelist changelist{this, &originKP, originRequestPredecessorKP, destinationRequestPredecessorKP, SAEVRouteChangelist::InsertionStatus::FAILURE_MIN};
    double detourScore = getDetourScore(originKP, originRequestPredecessorKP, destinationRequestPredecessorKP);
    //Properly insert the request to facilitate constraint propagation
    insertRequest(originKP, originRequestPredecessorKP, destinationRequestPredecessorKP);

    //Initialize bound propagation signal queue (each item signals a modification done on one of a KeyPoint
    std::queue<std::pair<Bound, SAEVKeyPoint *>> boundPropagationQueue{};
    SAEVKeyPoint * destinationKP = originKP.getCounterpart();
    boundPropagationQueue.emplace(Min, originKP.getPredecessor());
    boundPropagationQueue.emplace(Max, originKP.getSuccessor());
    boundPropagationQueue.emplace(Min, destinationKP->getPredecessor());
    boundPropagationQueue.emplace(Max, destinationKP->getSuccessor());
    boundPropagationQueue.emplace(Min, &originKP);
    boundPropagationQueue.emplace(Max, &originKP);
    boundPropagationQueue.emplace(Min, destinationKP);
    boundPropagationQueue.emplace(Max, destinationKP);

    //Pre-init variables used in the loop
    uint oldValue;
    uint newValue;
    SAEVKeyPoint * predecessorKP;
    SAEVKeyPoint * successorKP;
    SAEVKeyPoint * counterpartKP; //An Origin's Destination, or a Destination's Origin


    while(!boundPropagationQueue.empty()) {
        auto const& [bound, keyPoint] = boundPropagationQueue.front();
        boundPropagationQueue.pop();
        counterpartKP = keyPoint->getCounterpart();
        if(bound == Min) {
            successorKP = keyPoint->getSuccessor();
            if(successorKP != nullptr) {
                //Check neighbouring time window
                oldValue = successorKP->getMinTw();
                newValue = keyPoint->getMinTw() + _graph->getShortestSAEVPath(keyPoint->getNodeIndex(), successorKP->getNodeIndex());
                if (oldValue < newValue) {
                    if (newValue > successorKP->getMaxTw()) {
                        DEBUG_MSG("\tMIN TW VIOLATION");
                        changelist.setStatus(SAEVRouteChangelist::InsertionStatus::FAILURE_MIN);
                        return changelist;
                    }
                    changelist.emplace_back(*successorKP, Min, newValue - oldValue);
                    successorKP->setMinTw(newValue);
                    boundPropagationQueue.emplace(Min, successorKP);
                }
            }
            //Check counterpart key point delta time
            oldValue = counterpartKP->getMinTw();
            newValue = keyPoint->getMinTw() - keyPoint->getDeltaTime();
            if(!keyPoint->isDepot() && keyPoint->isDestination() && oldValue < newValue) {
                if (newValue > counterpartKP->getMaxTw()) {
                    DEBUG_MSG("\tMIN DELTA Destination->Origine");
                    changelist.setStatus(SAEVRouteChangelist::InsertionStatus::FAILURE_DELTA_MIN);
                    return changelist;
                }
                changelist.emplace_back(*counterpartKP, Min, newValue - oldValue);
                counterpartKP->setMinTw(newValue);
                boundPropagationQueue.emplace(Min, counterpartKP);
            }
        } else { //MAX
            predecessorKP = keyPoint->getPredecessor();
            if(predecessorKP != nullptr) {
                //Check neighbouring time window
                oldValue = predecessorKP->getMaxTw();
                newValue = keyPoint->getMaxTw() - _graph->getShortestSAEVPath(predecessorKP->getNodeIndex(), keyPoint->getNodeIndex());
                if(oldValue > newValue) {
                    if (predecessorKP->getMinTw() > newValue) {
                        DEBUG_MSG("\tMAX TW VIOLATION");
                        changelist.setStatus(SAEVRouteChangelist::InsertionStatus::FAILURE_MAX);
                        return changelist;
                    }
                    changelist.emplace_back(*predecessorKP, Max, newValue - oldValue);
                    predecessorKP->setMaxTw(newValue);
                    boundPropagationQueue.emplace(Max, predecessorKP);
                }
            }
            //Check counterpart key point delta time
            oldValue = counterpartKP->getMaxTw();
            newValue = keyPoint->getMaxTw() + keyPoint->getDeltaTime();
            if(!keyPoint->isDepot() && keyPoint->isOrigin() && oldValue > newValue) {
                if (counterpartKP->getMinTw() > newValue) {
                    DEBUG_MSG("\tMAX DELTA Origine->Destination");
                    changelist.setStatus(SAEVRouteChangelist::InsertionStatus::FAILURE_DELTA_MAX);
                    return changelist;
                }
                changelist.emplace_back(*counterpartKP, Max, oldValue - newValue);
                counterpartKP->setMaxTw(newValue);
                boundPropagationQueue.emplace(Max, counterpartKP);
            }
        }
    }

    changelist.setStatus(SAEVRouteChangelist::InsertionStatus::SUCCESS);
    changelist.setScore(detourScore);
    return changelist;
}

double SAEVRoute::getDetourScore(const SAEVKeyPoint &originKP, const SAEVKeyPoint * originRequestPredecessorKP,
                                 const SAEVKeyPoint * destinationRequestPredecessorKP) {
    double score;
    const SAEVKeyPoint* destinationKP = originKP.getCounterpart();
    const SAEVKeyPoint* originSuccKP = originRequestPredecessorKP->getSuccessor();
    const SAEVKeyPoint* destinationSuccKP = destinationRequestPredecessorKP->getSuccessor();

    if(originRequestPredecessorKP != destinationRequestPredecessorKP) {
        const SAEVKeyPoint* destinationPredKP = destinationRequestPredecessorKP;

        //Origin Detour
        score = _graph->getShortestSAEVPath(originRequestPredecessorKP->getNodeIndex(), originKP.getNodeIndex()) //T(Pred(O), D)
                + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccKP->getNodeIndex()) //T(O, Succ(D))
                - _graph->getShortestSAEVPath(originRequestPredecessorKP->getNodeIndex(), originSuccKP->getNodeIndex()); //T(Pred(O), Succ(O))

        //Destination Detour
        score += _graph->getShortestSAEVPath(destinationPredKP->getNodeIndex(), destinationKP->getNodeIndex()) //T(Pred(D), D))
                + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccKP->getNodeIndex()) //T(D, Succ(D)))
                - _graph->getShortestSAEVPath(destinationPredKP->getNodeIndex(), destinationSuccKP->getNodeIndex()); //T(Pred(D), Succ(D)))
    } else {
        score = _graph->getShortestSAEVPath(originRequestPredecessorKP->getNodeIndex(), originKP.getNodeIndex()) //T(Pred(O), O)
                + _graph->getShortestSAEVPath(originKP.getNodeIndex(), destinationKP->getNodeIndex()) //T(O, D)
                + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccKP->getNodeIndex()) //T(D, Succ(D))
                - _graph->getShortestSAEVPath(originRequestPredecessorKP->getNodeIndex(), destinationSuccKP->getNodeIndex()); //T(Pred(O), Succ(D))
    }
    return score;
}

std::string SAEVRoute::to_string(size_t vehicleId) {
    std::string routeString;
    SAEVKeyPoint const* currentKeyPoint = &getOriginDepot(vehicleId);
    while(currentKeyPoint != nullptr) {
        routeString += currentKeyPoint->to_string() + " --> ";
        currentKeyPoint = currentKeyPoint->getSuccessor();
    }
    routeString.erase(routeString.length() - 5, routeString.length());

    return routeString;
}

void SAEVRoute::exportToFile() {
    //TODO
}

bool SAEVRoute::checkRouteTimeWindows(size_t vehicleId) {
    SAEVKeyPoint const* currentKeyPoint = &getOriginDepot(vehicleId);
    SAEVKeyPoint const* succKP;
    SAEVKeyPoint const* counterpartKP;
    while(currentKeyPoint != nullptr) {
        succKP = currentKeyPoint->getSuccessor();
        counterpartKP = currentKeyPoint->getCounterpart();
        //MIN/MAX
        if(succKP != nullptr) {
            if(!currentKeyPoint->isDepot() && currentKeyPoint->getMinTw() + _graph->getShortestSAEVPath(currentKeyPoint->getNodeIndex(), succKP->getNodeIndex()) > succKP->getMaxTw() ) {
                DEBUG_MSG("MIN TW VIOLATION : " + currentKeyPoint->to_string() + " > " + succKP->to_string());
                return false;
            }
            if(!succKP->isDepot() && currentKeyPoint->getMaxTw() + _graph->getShortestSAEVPath(currentKeyPoint->getNodeIndex(), succKP->getNodeIndex()) < succKP->getMaxTw() ) {
                DEBUG_MSG("MAX TW VIOLATION : " + currentKeyPoint->to_string() + " < " + succKP->to_string());
                return false;
            }
        }
        //DELTA
        if((!currentKeyPoint->isDepot() && currentKeyPoint->isOrigin() && counterpartKP != nullptr)
        && (counterpartKP->getMinTw() - currentKeyPoint->getMinTw() > currentKeyPoint->getRequest()->getDeltaTime()
        ||  counterpartKP->getMaxTw() - currentKeyPoint->getMaxTw() > currentKeyPoint->getRequest()->getDeltaTime())) {
            DEBUG_MSG("DELTA VIOLATION : " + currentKeyPoint->to_string() + " " + counterpartKP->to_string());
            return false;
        }
        currentKeyPoint = currentKeyPoint->getSuccessor();
    }

    return true;
}

/** TODO: move to BestInsertionQueue class with a route parameter ?
 * Initializes a BestInsertionQueue to guide the best insertion heuristic
 * @param requestOriginKeyPoint Reference to the origin key point
 * @param vehicleId The ID of the vehicle in which we look for new best insertions
 * @return The created BestInsertionQueue filled with the best insertions for the given request and vehicle IDs
 */
BestInsertionQueue SAEVRoute::getBestInsertionsQueue(size_t requestId, size_t vehicleId) {
    return getBestInsertionsQueue(getRequestOrigin(requestId), vehicleId);
}

/** TODO: move to BestInsertionQueue class with a route parameter ?
 * Initializes a BestInsertionQueue to guide the best insertion heuristic
 * @param requestOriginKeyPoint Reference to the origin key point
 * @param vehicleId The ID of the vehicle in which we look for new best insertions
 * @return The created BestInsertionQueue filled with the best insertions for the given request and vehicle IDs
 */
BestInsertionQueue SAEVRoute::getBestInsertionsQueue(SAEVKeyPoint &originKP, size_t vehicleId) {
    BestInsertionQueue bestInsertionQueue(originKP, vehicleId, _nbRequest ^ 2);

    //Init variables used during iteration
    double score;
    SAEVKeyPoint * originInsertionKeyPoint = &getOriginDepot(vehicleId);
    SAEVKeyPoint * destinationInsertionKeyPoint = originInsertionKeyPoint;

    //iterate over possible origin/destination pairs for the given vehicle
    while(originInsertionKeyPoint->getSuccessor() != nullptr) {
        while(destinationInsertionKeyPoint->getSuccessor() != nullptr) {
            score = getDetourScore(originKP, originInsertionKeyPoint, destinationInsertionKeyPoint);
            bestInsertionQueue.emplace(originInsertionKeyPoint, destinationInsertionKeyPoint, score);
            destinationInsertionKeyPoint = destinationInsertionKeyPoint->getSuccessor();
        }

        //Iterate over possible origins and reset destination to being the same point as the origin
        originInsertionKeyPoint = originInsertionKeyPoint->getSuccessor();
        destinationInsertionKeyPoint = originInsertionKeyPoint;
    }

    return bestInsertionQueue;
}

/**
 * Initializes a BestInsertionQueue to guide the best insertion heuristic while checking neighbouring TW checks
 * @param requestId The ID of the base request for which we search best insertions
 * @param vehicleId The ID of the vehicle in which we look for new best insertions
 * @return The created BestInsertionQueue filled with the best insertions for the given request and vehicle IDs
 */
BestInsertionQueue SAEVRoute::getBestFeasibleInsertionsQueue(size_t requestId, size_t vehicleId) {
    return getBestFeasibleInsertionsQueue(getRequestOrigin(requestId), vehicleId);
}

/**
 * Initializes a BestInsertionQueue to guide the best insertion heuristic while checking neighbouring TW checks
 * @param requestOriginKeyPoint Reference to the origin key point of the request for which we search best insertions
 * @param vehicleId The ID of the vehicle in which we look for new best insertions
 * @return The created BestInsertionQueue filled with the best insertions for the given request and vehicle IDs
 */
BestInsertionQueue SAEVRoute::getBestFeasibleInsertionsQueue(SAEVKeyPoint &originKP, size_t vehicleId) {
    BestInsertionQueue bestInsertionQueue(originKP, vehicleId, _nbRequest ^ 2);
    getBestFeasibleInsertionsQueue(bestInsertionQueue, originKP, vehicleId);
    return bestInsertionQueue;
}

/**
 * Search for new best insertions to add into the given best insertion queue for a given request and vehicle
 * @param bestInsertionQueue The queue we wish to add the new feasible best insertions to
 * @param requestOriginKeyPoint Reference to the origin key point of the request for which we search best insertions
 * @param vehicleId The ID of the vehicle in which we look for new best insertions
 */
void SAEVRoute::getBestFeasibleInsertionsQueue(BestInsertionQueue& bestInsertionQueue, const SAEVKeyPoint &requestOriginKeyPoint, size_t vehicleId) {
    //Init variables used during iteration
    double score;
    SAEVKeyPoint * originInsertionKeyPoint = &getOriginDepot(vehicleId);
    SAEVKeyPoint * destinationInsertionKeyPoint = originInsertionKeyPoint;

    //iterate over possible origin/destination pairs for the given vehicle
    while(originInsertionKeyPoint->getSuccessor() != nullptr) {
        while(destinationInsertionKeyPoint->getSuccessor() != nullptr) {
            if(doNeighbouringTWChecks(requestOriginKeyPoint, originInsertionKeyPoint, destinationInsertionKeyPoint)) {
                score = getDetourScore(requestOriginKeyPoint, originInsertionKeyPoint, destinationInsertionKeyPoint);
                bestInsertionQueue.emplace(originInsertionKeyPoint, destinationInsertionKeyPoint, score);
            }
            destinationInsertionKeyPoint = destinationInsertionKeyPoint->getSuccessor();
        }

        //Iterate over possible origins and reset destination to being the same point as the origin
        originInsertionKeyPoint = originInsertionKeyPoint->getSuccessor();
        destinationInsertionKeyPoint = originInsertionKeyPoint;
    }
}

void SAEVRoute::addRequestWeightToRoute(SAEVKeyPoint &requestOriginKeyPoint) {
    SAEVKeyPoint* currentKP = &requestOriginKeyPoint;
    uint requestWeight = currentKP->getRequest()->getWeight();
    currentKP->setCurrentOccupation(currentKP->getPredecessor()->getCurrentOccupation() + requestWeight); //O.Weight = Prec(O).Weight + R.Weight (request enters the vehicle=)
    do {
        currentKP = currentKP->getSuccessor();
        currentKP->setCurrentOccupation(currentKP->getCurrentOccupation() + requestWeight);
    } while (currentKP != requestOriginKeyPoint.getCounterpart());
    currentKP->setCurrentOccupation(currentKP->getPredecessor()->getCurrentOccupation() - requestWeight); //D.Weight = Prec(D).Weight - R.Weight (request leaves the vehicle)
}

void SAEVRoute::removeRequestWeightFromRoute(SAEVKeyPoint& requestOriginKeyPoint) {
    SAEVKeyPoint* currentKP = &requestOriginKeyPoint;
    uint requestWeight = currentKP->getRequest()->getWeight();
    currentKP->setCurrentOccupation(0); //reset request weight on origin KP
    do {
        currentKP = currentKP->getSuccessor();
        currentKP->setCurrentOccupation(currentKP->getCurrentOccupation() - requestWeight);
    } while (currentKP != requestOriginKeyPoint.getCounterpart());
    currentKP->setCurrentOccupation(0); //reset request weight on destination KP
}

void SAEVRoute::insertRequestInNewVehicle(SAEVKeyPoint &originKp) {
    size_t vehicleId = addNewActiveVehicle();
    DEBUG_MSG("NEW VEHICLE CREATED, ID :" + std::to_string(vehicleId));

    tryAddRequest(originKp, &getOriginDepot(vehicleId), &getOriginDepot(vehicleId));
}

