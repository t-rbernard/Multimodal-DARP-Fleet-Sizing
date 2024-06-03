//
// Created by romain on 22/03/24.
//

#include "SAEVKeyPoint.h"

bool SAEVKeyPoint::check() const {
    return KeyPoint::check();
}

SAEVKeyPoint *SAEVKeyPoint::getPredecessor() const {
    return _predecessor;
}

void SAEVKeyPoint::setPredecessor(SAEVKeyPoint *predecessor) {
    _predecessor = predecessor;
}

SAEVKeyPoint *SAEVKeyPoint::getSuccessor() const {
    return _successor;
}

void SAEVKeyPoint::setSuccessor(SAEVKeyPoint *successor) {
    _successor = successor;
}

int SAEVKeyPoint::getCurrentOccupation() const {
    return _currentOccupation;
}

void SAEVKeyPoint::setCurrentOccupation(int currentCapacity) {
    _currentOccupation = currentCapacity;
}

int SAEVKeyPoint::getMinTw() const {
    return _minTW;
}

void SAEVKeyPoint::setMinTw(int minTw) {
    _minTW = minTw;
}

int SAEVKeyPoint::getMaxTw() const {
    return _maxTW;
}

void SAEVKeyPoint::setMaxTw(int maxTw) {
    _maxTW = maxTw;
}

SAEVKeyPoint::SAEVKeyPoint(const Graph &graph, const Request &request, bool isOrigin) : _isOrigin(isOrigin), _requestPointer(&request) {
    if(isOrigin) {
        setNodeIndex(request.getOriginNodeIndex());
        _minTW = request.getArrivalTw().min - request.getDeltaTime();
        _maxTW = request.getArrivalTw().max - graph.getShortestSAEVPath(request.getOriginNodeIndex(),
                                                                        request.getDestinationNodeIndex());
    } else {
        setNodeIndex(request.getDestinationNodeIndex());
        _minTW = request.getArrivalTw().min;
        _maxTW = request.getArrivalTw().max;
    }
}

bool SAEVKeyPoint::operator==(const SAEVKeyPoint &rhs) const {
    return this == &rhs;
}

bool SAEVKeyPoint::isOrigin() const {
    return _isOrigin;
}

void SAEVKeyPoint::setIsOrigin(bool isOrigin) {
    _isOrigin = isOrigin;
}

Request const *SAEVKeyPoint::getRequest() const {
    return _requestPointer;
}

int SAEVKeyPoint::getDeltaTime() const {
    return _isDepot ? 0 : _requestPointer->getDeltaTime();
}

void SAEVKeyPoint::setRequest(const Request *requestPointer) {
    _requestPointer = requestPointer;
}

SAEVKeyPoint *SAEVKeyPoint::getCounterpart() const {
    return _counterpart;
}

void SAEVKeyPoint::setCounterpart(SAEVKeyPoint *counterpart) {
    _counterpart = counterpart;
}

SAEVKeyPoint::SAEVKeyPoint(int depotNodeIdx) : KeyPoint(depotNodeIdx) {
    _isDepot = true;
    setNodeIndex(depotNodeIdx);
}

bool SAEVKeyPoint::isDepot() const {
    return _isDepot;
}

void SAEVKeyPoint::setIsDepot(bool isDepot) {
    _isDepot = isDepot;
}
