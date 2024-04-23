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
        _route.at(i*2) = SAEVKeyPoint(true); //start
        _route.at(i*2+1) = SAEVKeyPoint(false); //end
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


