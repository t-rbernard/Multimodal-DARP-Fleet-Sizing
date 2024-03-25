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

bool SAEVKeyPoint::isEntry() const {
    return _isEntry;
}

void SAEVKeyPoint::setIsEntry(bool isEntry) {
    SAEVKeyPoint::_isEntry = isEntry;
}

bool SAEVKeyPoint::check() const {
    return KeyPoint::check() && _request != nullptr;
}
