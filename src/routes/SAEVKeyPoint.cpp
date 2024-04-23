//
// Created by romain on 22/03/24.
//

#include "SAEVKeyPoint.h"


SAEVKeyPoint::SAEVKeyPoint(Request *request, bool isEntry) {
    _request = request;
    _isEntry = isEntry;
}

Request *SAEVKeyPoint::getRequest() const {
    return _request;
}

void SAEVKeyPoint::setRequest(Request *request) {
    SAEVKeyPoint::_request = request;
}

bool SAEVKeyPoint::check() const {
    return KeyPoint::check() && _request != nullptr;
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
    return _currentCapacity;
}

void SAEVKeyPoint::setCurrentCapacity(int currentCapacity) {
    _currentCapacity = currentCapacity;
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
