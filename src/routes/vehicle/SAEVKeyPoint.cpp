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

uint SAEVKeyPoint::getMinTw() const {
    return _minTW;
}

void SAEVKeyPoint::setMinTw(uint minTw) {
    _minTW = minTw;
}

uint SAEVKeyPoint::getMaxTw() const {
    return _maxTW;
}

void SAEVKeyPoint::setMaxTw(uint maxTw) {
    _maxTW = maxTw;
}

SAEVKeyPoint::SAEVKeyPoint(const Graph &graph, const Request &request, bool isOrigin) : _isOrigin(isOrigin), _requestPointer(&request) {
    if(isOrigin) {
        setNodeIndex(request.getOriginNodeIndex());
        _minTW = request.getDepartureTw().min;
        _maxTW = request.getDepartureTw().max;
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

bool SAEVKeyPoint::isDestination() const {
    return !_isOrigin;
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

std::string SAEVKeyPoint::to_string() const {
    std::string nodeType = "Depot, ";
    if(!isDepot() && isOrigin()) { nodeType = "Origin, ";}
    else if(!isDepot()) { nodeType = "Destination,"; }

    return "(nodeIdx=" + std::to_string(getNodeIndex()) + ", Type=" + nodeType +
    + "TW=[" + std::to_string(getMinTw()) + ";" + std::to_string(getMaxTw()) + "])";
}
