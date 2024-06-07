//
// Created by romain on 22/03/24.
//

#include <queue>
#include <iostream>
#include "SAEVRoute.h"

#ifdef DEBUG_TRANSIT_PRECOMPUTE
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

SAEVRoute::SAEVRoute(const Graph& graph, const std::vector<Request>& requestList) : _nbRequest(requestList.size()), _graph(&graph), _requestList(&requestList) {
    _route.resize(_nbRequest*4); //nbRequest*2 O/D KeyPoints + nbRequest*2 Start/End depots (upper bound #vehicle = #requests

    //Init Request O/D and Depot start/end key points
    for(size_t i = 0; i < _nbRequest; ++i) {
        //Create key O/D points
        getOrigin(i) = SAEVKeyPoint(graph, requestList.at(i), true); //origin
        getDestination(i) = SAEVKeyPoint(graph, requestList.at(i), false); //destination
        //Link Origins and Destinations
        getOrigin(i).setCounterpart(&getDestination(i));
        getDestination(i).setCounterpart(&getOrigin(i));

        //Create depot O/D KP (Upper Bound = nb requests)
        getOriginDepot(i) = SAEVKeyPoint(graph.getDepotNodeIdx()); //start
        getDestinationDepot(i) = SAEVKeyPoint(graph.getDepotNodeIdx()); //end
        //Link Origin depots and Destination depots
        getOriginDepot(i).setCounterpart(&getDestinationDepot(i));
        getDestinationDepot(i).setCounterpart(&getOriginDepot(i));
        getOriginDepot(i).setSuccessor(&getDestinationDepot(i));
        getDestinationDepot(i).setPredecessor(&getOriginDepot(i));
    }
}

void SAEVRoute::insertRequest(size_t requestId, size_t originRequestPredecessorIdx, size_t destinationRequestPredecessorIdx) {
    SAEVKeyPoint& originKp = getOrigin(requestId);
    SAEVKeyPoint& destinationKp = getDestination(requestId);

    SAEVKeyPoint& originPredKp = _route.at(originRequestPredecessorIdx);
    SAEVKeyPoint *originSuccKp = originPredKp.getSuccessor();
    SAEVKeyPoint& destinationPredKp = _route.at(destinationRequestPredecessorIdx);
    SAEVKeyPoint *destinationSuccKp = destinationPredKp.getSuccessor();

    if(originRequestPredecessorIdx != destinationRequestPredecessorIdx) {
        //Set values for O/D KPs
        originKp.setPredecessor(&originPredKp);
        originKp.setSuccessor(originSuccKp);
        destinationKp.setPredecessor(&destinationPredKp);
        destinationKp.setSuccessor(destinationSuccKp);

        //Set values for predecessors/successors
        originPredKp.setSuccessor(&originKp);
        originSuccKp->setPredecessor(&originKp);
        destinationPredKp.setSuccessor(&destinationKp);
        destinationSuccKp->setPredecessor(&destinationKp);
    } else {
        //Set values for O/D KPs
        originKp.setPredecessor(&originPredKp);
        originKp.setSuccessor(&destinationKp);
        destinationKp.setPredecessor(&originKp);
        destinationKp.setSuccessor(destinationSuccKp);

        //Set values for predecessors/successors
        originPredKp.setSuccessor(&originKp);
        originSuccKp->setPredecessor(&destinationKp);
    }
}

void SAEVRoute::removeRequest(int requestId) {
    SAEVKeyPoint& originKp = _route.at(requestId);
    SAEVKeyPoint& destinationKp = _route.at(requestId + 1);

    //get predecessor and successor for request
    SAEVKeyPoint* originPredecessor = originKp.getPredecessor();
    SAEVKeyPoint* originSuccessor = destinationKp.getSuccessor();
    SAEVKeyPoint* destinationPredecessor = originKp.getPredecessor();
    SAEVKeyPoint* destinationSuccessor = destinationKp.getSuccessor();

    //Link pred and successor from origin and destination
    originPredecessor->setSuccessor(originSuccessor);
    destinationPredecessor->setSuccessor(destinationSuccessor);

    //Revert origin/destination key points to their default state
    originKp.setPredecessor(nullptr);
    originKp.setSuccessor(nullptr);
    destinationKp.setPredecessor(nullptr);
    destinationKp.setSuccessor(nullptr);
    originKp.setMinTw(_requestList->at(requestId).getMinDepartureTw());
    originKp.setMaxTw(_requestList->at(requestId).getMaxDepartureTw());
    destinationKp.setMinTw(_requestList->at(requestId).getMinArrivalTw());
    destinationKp.setMaxTw(_requestList->at(requestId).getMaxArrivalTw());
}

SAEVRouteChangelist
SAEVRoute::tryAddRequest(const size_t requestId, const size_t originRequestPredecessorIdx, const size_t destinationRequestPredecessorIdx) {
    const Request* request = &_requestList->at(requestId);
    SAEVKeyPoint const* originPredecessor = &_route.at(originRequestPredecessorIdx);
    SAEVKeyPoint const* destinationPredecessor = &_route.at(destinationRequestPredecessorIdx);

    //Check vehicle capacity
    SAEVKeyPoint const* currentKP = originPredecessor;
    do {
        if(currentKP->getCurrentOccupation() + request->getWeight() > SAEVehicle::getCapacity()) {
            return SAEVRouteChangelist(this, requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx);
        }
        currentKP = currentKP->getSuccessor();
    } while (currentKP != destinationPredecessor && currentKP != nullptr);

    //Do basic checks on neighbouring nodes from our Origin/Destination insertion points
    bool isValid = doNeighbouringTWChecks(requestId, request->getOriginNodeIndex(), request->getDestinationNodeIndex(), originPredecessor, destinationPredecessor);

    if(isValid)
        return insertRequestWithPropagation(requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx);
    else
        return SAEVRouteChangelist(this, requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx);

}

bool
SAEVRoute::doNeighbouringTWChecks(const size_t requestId, const size_t originNodeIndex, const size_t destinationNodeIndex,
                                  const SAEVKeyPoint *originPredecessor, const SAEVKeyPoint *destinationPredecessor) {

    const SAEVKeyPoint& originKP = getOrigin(requestId);
    const SAEVKeyPoint& destinationKP = getDestination(requestId);
    const SAEVKeyPoint* originSuccessor = originPredecessor->getSuccessor();

    if(originPredecessor != destinationPredecessor)
    {
        SAEVKeyPoint const* destinationSuccessor = destinationPredecessor->getSuccessor();

        //Tests time windows Origin (yes this if-else could be one giant OR, but I'd rather separate every case)
        int predOriginTimeWindow = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), originNodeIndex);
        int predDestinationTimeWindow = destinationPredecessor->getMinTw() + _graph->getShortestSAEVPath(destinationPredecessor->getNodeIndex(), destinationNodeIndex);
        if(predOriginTimeWindow > originKP.getMaxTw())
            return false;
        else if(originKP.getMinTw() + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) // Could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
        //Tests time windows Destination
        else if(predDestinationTimeWindow > destinationKP.getMaxTw())
            return false;
        else if(destinationKP.getMinTw() + _graph->getShortestSAEVPath(destinationKP.getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw()) //could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(destinationKP.getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw())
            return false;
    } else { //We need a specific case if origin and destination are inserted after the same node
        int predMinTWToOrigin = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), originNodeIndex);
        int predMinTWToDest = predMinTWToOrigin + _graph->getShortestSAEVPath(originNodeIndex, destinationNodeIndex);
        if(predMinTWToOrigin > originKP.getMaxTw()) //Path from pred to O
            return false;
        else if(predMinTWToDest > destinationKP.getMaxTw()) //Path from pred to D
            return false;
        else if(predMinTWToDest + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from pred to successor
            return false;
        else if(originSuccessor->getMinTw() + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from D to successor
            return false;
        else if(originKP.getMinTw() + _graph->getShortestSAEVPath(originNodeIndex, destinationNodeIndex) //Path from O to successor
        + _graph->getShortestSAEVPath(destinationNodeIndex, originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
    }

    return true;
}

SAEVRouteChangelist SAEVRoute::insertRequestWithPropagation(const size_t requestId, const size_t originRequestPredecessorIdx,
                                                            const size_t destinationRequestPredecessorIdx) {
    //Init changelist
    SAEVRouteChangelist changelist{this, requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx};
    //Properly insert the request to facilitate constraint propagation
    insertRequest(requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx);

    //Initialize bound propagation signal queue (each item signals a modification done on one of a KeyPoint
    std::queue<std::pair<Bound, SAEVKeyPoint *>> boundPropagationQueue{};
    SAEVKeyPoint * originKP = &getOrigin(requestId);
    SAEVKeyPoint * destinationKP = &getDestination(requestId);
    boundPropagationQueue.emplace(Min, originKP);
    boundPropagationQueue.emplace(Max, originKP);
    boundPropagationQueue.emplace(Min, destinationKP);
    boundPropagationQueue.emplace(Max, destinationKP);

    //Pre-init variables used in the loop
    int oldValue;
    int newValue;
    SAEVKeyPoint * predecessorKP;
    SAEVKeyPoint * successorKP;
    SAEVKeyPoint * counterpartKP; //An Origin's Destination, or a Destination's Origin


    while(!boundPropagationQueue.empty()) {
        auto const& [bound, keyPoint] = boundPropagationQueue.front();
        boundPropagationQueue.pop();
        counterpartKP = keyPoint->getCounterpart();
        DEBUG_MSG("KP=" + keyPoint->to_string() + "\n");
        if(bound == Min) {
            successorKP = keyPoint->getSuccessor();
            if(successorKP != nullptr) {
                //Check neighbouring time window
                oldValue = successorKP->getMinTw();
                newValue = keyPoint->getMinTw() + _graph->getShortestSAEVPath(keyPoint->getNodeIndex(), successorKP->getNodeIndex());
                if (oldValue < newValue) {
                    if (newValue > successorKP->getMaxTw()) {
                        return changelist;
                    }
                    DEBUG_MSG("\tMIN Successeur KP=" + successorKP->to_string() + "\n\tModif Min=" + std::to_string(oldValue) + "->" + std::to_string(newValue));
                    changelist.emplace_back(*successorKP, Min, newValue - oldValue);
                    successorKP->setMinTw(newValue);
                    boundPropagationQueue.emplace(Min, successorKP);
                } else {
//                    DEBUG_MSG("Pas de changement pour Min du noeud " + successorKP->to_string() + " successeur de " + keyPoint->to_string());
                }
            }
            //Check counterpart key point delta time
            oldValue = counterpartKP->getMinTw();
            newValue = keyPoint->getMinTw() - keyPoint->getDeltaTime();
            if(!keyPoint->isDepot() && !counterpartKP->isOrigin() && oldValue < newValue) {
                if (newValue > counterpartKP->getMaxTw()) {
                    DEBUG_MSG("Infaisabilité MIN DELTA Destination->Origine");
                    return changelist;
                }
                DEBUG_MSG("\tMIN Counterpart KP=" + counterpartKP->to_string() + "\n\tModif Min=" + std::to_string(oldValue) + "->" + std::to_string(newValue));
                changelist.emplace_back(*counterpartKP, Min, newValue - oldValue);
                counterpartKP->setMinTw(newValue);
                boundPropagationQueue.emplace(Min, counterpartKP);
            } else if(!keyPoint->isDepot() && keyPoint->isDestination()) {
//                DEBUG_MSG("Pas de changement pour Min du noeud " + counterpartKP->to_string() + " via  sa destination " + keyPoint->to_string());
            }
        } else { //MAX
            predecessorKP = keyPoint->getPredecessor();
            if(predecessorKP != nullptr) {
                //Check neighbouring time window
                oldValue = predecessorKP->getMaxTw();
                newValue = keyPoint->getMaxTw() - _graph->getShortestSAEVPath(predecessorKP->getNodeIndex(), keyPoint->getNodeIndex());
                if(oldValue > newValue) {
                    if (predecessorKP->getMinTw() > newValue) {
                        return changelist;
                    }
                    DEBUG_MSG("\tMAX Predecessor KP=" + predecessorKP->to_string() + "\n\tModif Max=" + std::to_string(oldValue) + "->" + std::to_string(newValue));
                    changelist.emplace_back(*predecessorKP, Max, newValue - oldValue);
                    predecessorKP->setMaxTw(newValue);
                    boundPropagationQueue.emplace(Max, predecessorKP);
                } else {
//                    DEBUG_MSG("Pas de changement pour Max pour noeud " + predecessorKP->to_string() + " prédécesseur de " + predecessorKP->to_string());
                }
            }
            //Check counterpart key point delta time
            oldValue = counterpartKP->getMaxTw();
            newValue = keyPoint->getMaxTw() + keyPoint->getDeltaTime();
            if(!keyPoint->isDepot() && counterpartKP->isOrigin() && oldValue > newValue) {
                if (counterpartKP->getMinTw() > newValue) {
                    DEBUG_MSG("Infaisabilité MAX DELTA Origine->Destination");
                    return changelist;
                }
                changelist.emplace_back(*counterpartKP, Max, oldValue - newValue);
                counterpartKP->setMaxTw(newValue);
                boundPropagationQueue.emplace(Max, counterpartKP);
                DEBUG_MSG("\tMAX Destination KP=" + counterpartKP->to_string() + "\n\tModif Max=" + std::to_string(oldValue) + "->" + std::to_string(newValue));
            } else if(!keyPoint->isDepot() && keyPoint->isOrigin()) {
//                DEBUG_MSG("Pas de changement pour Max du noeud " + counterpartKP->to_string() + " via son origine " + keyPoint->to_string());
            }
        }
    }

    changelist.setScore(getDetourScore(requestId, originRequestPredecessorIdx, destinationRequestPredecessorIdx));
    return changelist;
}

double SAEVRoute::getDetourScore(const size_t requestId, const size_t originRequestPredecessorIdx,
                                 const size_t destinationRequestPredecessorIdx) {
    double score;
    const SAEVKeyPoint& originKP = getOrigin(requestId);
    const SAEVKeyPoint& destinationKP = getDestination(requestId);
    const SAEVKeyPoint& originPredKP = _route.at(originRequestPredecessorIdx);
    const SAEVKeyPoint* originSuccKP = originPredKP.getSuccessor();

    if(originRequestPredecessorIdx != destinationRequestPredecessorIdx) {
        const SAEVKeyPoint* destinationPredKP = getDestination(requestIdx).getPredecessor();
        const SAEVKeyPoint* destinationSuccKP = getDestination(requestIdx).getSuccessor();

        //Origin Detour
        score = _graph->getShortestSAEVPath(originPredKP.getNodeIndex(), originKP.getNodeIndex()) //T(Pred(O), D)
                + _graph->getShortestSAEVPath(originKP.getNodeIndex(), originSuccKP->getNodeIndex()) //T(O, Succ(D))
                - _graph->getShortestSAEVPath(originPredKP.getNodeIndex(), originSuccKP->getNodeIndex()); //T(Pred(O), Succ(O))

        //Destination Detour
        score += _graph->getShortestSAEVPath(destinationPredKP->getNodeIndex(), destinationKP.getNodeIndex()) //T(Pred(D), D))
                + _graph->getShortestSAEVPath(destinationKP.getNodeIndex(), destinationSuccKP->getNodeIndex()) //T(D, Succ(D)))
                - _graph->getShortestSAEVPath(destinationPredKP->getNodeIndex(), destinationSuccKP->getNodeIndex()); //T(Pred(D), Succ(D)))
    } else {
        score = _graph->getShortestSAEVPath(originPredKP.getNodeIndex(), originKP.getNodeIndex()) //T(Pred(O), O)
                + _graph->getShortestSAEVPath(originKP.getNodeIndex(), destinationKP.getNodeIndex()) //T(O, D)
                + _graph->getShortestSAEVPath(destinationKP.getNodeIndex(), originSuccKP->getNodeIndex()) //T(D, Succ(D))
                - _graph->getShortestSAEVPath(originPredKP.getNodeIndex(), originSuccKP->getNodeIndex()); //T(Pred(O), Succ(D))
    }
    return score;
}

std::string SAEVRoute::to_string(size_t vehicleId) {
    std::string routeString;
    SAEVKeyPoint* currentKeyPoint = &getOriginDepot(vehicleId);
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

