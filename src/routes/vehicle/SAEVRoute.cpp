//
// Created by romain on 22/03/24.
//

#include "SAEVRoute.h"

SAEVRoute::SAEVRoute(const Graph& graph, const std::vector<Request>& requestList) : _nbRequest(requestList.size()), _graph(&graph), _requestList(&requestList) {
    _route.reserve(_nbRequest*4); //nbRequest*2 O/D KeyPoints + nbRequest*2 Start/End depots (upper bound #vehicle = #requests

    //Init Request O/D key points
    for(size_t i = 0; i < _nbRequest; ++i) {
        _route.at(i*2) = SAEVKeyPoint(graph, requestList.at(i), true); //origin
        _route.at(i*2+1) = SAEVKeyPoint(graph, requestList.at(i), false); //destination
    }

    //Init start/end depot key points
    for(size_t i = _nbRequest; i < _nbRequest*2; ++i) {
        _route.at(i*2) = SAEVKeyPoint(); //start
        _route.at(i*2+1) = SAEVKeyPoint(); //end
    }
}

void SAEVRoute::insertRequest(int requestIdx, int originRequestPredecessorIdx, int destinationRequestPredecessorIdx) {
    SAEVKeyPoint& originKp = _route.at(requestIdx);
    SAEVKeyPoint& destinationKp = _route.at(requestIdx + 1);

    SAEVKeyPoint& originPredKp = _route.at(originRequestPredecessorIdx);
    SAEVKeyPoint *originSuccKp = originPredKp.getSuccessor();
    SAEVKeyPoint& destinationPredKp = _route.at(destinationRequestPredecessorIdx);
    SAEVKeyPoint *destinationSuccKp = destinationPredKp.getSuccessor();

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
}

void SAEVRoute::removeRequest(int requestIdx) {
    SAEVKeyPoint& originKp = _route.at(requestIdx);
    SAEVKeyPoint& destinationKp = _route.at(requestIdx + 1);

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
    originKp.setMinTw(_requestList->at(requestIdx).getMinDepartureTw());
    originKp.setMaxTw(_requestList->at(requestIdx).getMaxDepartureTw());
    destinationKp.setMinTw(_requestList->at(requestIdx).getMinArrivalTw());
    destinationKp.setMaxTw(_requestList->at(requestIdx).getMaxArrivalTw());
}

SAEVRouteChangelist
SAEVRoute::tryAddRequest(const int requestIdx, const int originRequestPredecessorIdx, const int destinationRequestPredecessorIdx) {
    const Request* request = &_requestList->at(requestIdx);
    SAEVKeyPoint const* originPredecessor = &_route.at(originRequestPredecessorIdx);
    SAEVKeyPoint const* destinationPredecessor = &_route.at(destinationRequestPredecessorIdx);

    //Check vehicle capacity
    SAEVKeyPoint const* currentKP = originPredecessor;
    do {
        if(currentKP->getCurrentOccupation() + request->getWeight() > SAEVehicle::getCapacity()) {
            return SAEVRouteChangelist(this, requestIdx, originRequestPredecessorIdx, destinationRequestPredecessorIdx);
        }
        currentKP = currentKP->getSuccessor();
    } while (currentKP != destinationPredecessor && currentKP != nullptr);

    //Do basic checks on neighbouring nodes from our Origin/Destination insertion points
    bool isValid = doNeighbouringTWChecks(requestIdx, request, originPredecessor, destinationPredecessor);

    if(isValid)
        return insertRequestWithPropagation(requestIdx, originRequestPredecessorIdx,destinationRequestPredecessorIdx);
    else
        return SAEVRouteChangelist(this, requestIdx, originRequestPredecessorIdx, destinationRequestPredecessorIdx);

}

SAEVRouteChangelist SAEVRoute::insertRequestWithPropagation(const int requestIdx, const int originRequestPredecessorIdx,
                                                            const int destinationRequestPredecessorIdx) {
    return SAEVRouteChangelist(nullptr, 0, 0, 0);
}

bool
SAEVRoute::doNeighbouringTWChecks(const int requestIdx,  const Request* request,
                                  const SAEVKeyPoint *originPredecessor, const SAEVKeyPoint *destinationPredecessor) {

    SAEVKeyPoint const* originKP = &_route.at(requestIdx);
    SAEVKeyPoint const* destinationKP = &_route.at(requestIdx + 1);
    SAEVKeyPoint const* originSuccessor = originPredecessor->getSuccessor();

    if(originPredecessor != destinationPredecessor)
    {

        SAEVKeyPoint const* destinationSuccessor = destinationPredecessor->getSuccessor();

        //Tests time windows Origin (yes this if-else could be one giant OR, but I'd rather separate every case)
        int predOriginTimeWindow = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), request->getDepartureNodeIndex());
        int predDestinationTimeWindow = destinationPredecessor->getMinTw() + _graph->getShortestSAEVPath(destinationPredecessor->getNodeIndex(), request->getArrivalNodeIndex());
        if(predOriginTimeWindow > originKP->getMaxTw())
            return false;
        else if(originKP->getMinTw() + _graph->getShortestSAEVPath(originKP->getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) // Could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(originKP->getNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
        //Tests time windows Destination
        else if(predDestinationTimeWindow > destinationKP->getMaxTw())
            return false;
        else if(destinationKP->getMinTw() + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw()) //could be removed ?
            return false;
        else if(predOriginTimeWindow + _graph->getShortestSAEVPath(destinationKP->getNodeIndex(), destinationSuccessor->getNodeIndex()) > destinationSuccessor->getMaxTw())
            return false;
    } else {
        int predMinTWToOrigin = originPredecessor->getMinTw() + _graph->getShortestSAEVPath(originPredecessor->getNodeIndex(), request->getDepartureNodeIndex());
        int predMinTWToDest = predMinTWToOrigin + _graph->getShortestSAEVPath(request->getDepartureNodeIndex(), request->getArrivalNodeIndex());
        if(predMinTWToOrigin > originKP->getMaxTw()) //Path from pred to O
            return false;
        else if(predMinTWToDest > destinationKP->getMaxTw()) //Path from pred to D
            return false;
        else if(predMinTWToDest + _graph->getShortestSAEVPath(request->getArrivalNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from pred to successor
            return false;
        else if(originSuccessor->getMinTw() + _graph->getShortestSAEVPath(request->getArrivalNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw()) //Path from D to successor
            return false;
        else if(originKP->getMinTw() + _graph->getShortestSAEVPath(request->getDepartureNodeIndex(), request->getArrivalNodeIndex()) //Path from O to successor
        + _graph->getShortestSAEVPath(request->getArrivalNodeIndex(), originSuccessor->getNodeIndex()) > originSuccessor->getMaxTw())
            return false;
    }

    return true;
}


