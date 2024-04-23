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

int SAEVKeyPoint::getCurrentCapacity() const {
    return _currentOccupation;
}

void SAEVKeyPoint::setCurrentCapacity(int currentCapacity) {
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

SAEVKeyPoint::SAEVKeyPoint(const Graph &graph, const Request &request, bool isEntry) {
    if(isEntry) {
        _minTW = request.getArrivalTw().min - request.getDeltaTime();
        _maxTW = request.getArrivalTw().max - graph.getShortestSAEVPath(request.getDepartureNodeIndex(), request.getArrivalNodeIndex());
    } else {
        _minTW = request.getArrivalTw().min;
        _maxTW = request.getArrivalTw().max;
    }
}

SAEVKeyPoint::SAEVKeyPoint(bool isEntry) {
    if(isEntry) {
        _minTW =
        _maxTW = request.getArrivalTw().max - graph.getShortestSAEVPath(request.getDepartureNodeIndex(), request.getArrivalNodeIndex());
    } else {
        _minTW = request.getArrivalTw().min;
        _maxTW = request.getArrivalTw().max;
    }
}
