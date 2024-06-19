//
// Created by romain on 23/04/24.
//

#include "SAEVRouteChangelist.h"

const std::vector<SAEVRouteChange> &SAEVRouteChangelist::getChangelist() const {
    return _changelist;
}

void SAEVRouteChangelist::push_back(SAEVRouteChange change) {
    _changelist.push_back(change);
}

void SAEVRouteChangelist::emplace_back(SAEVRouteChange change) {
    _changelist.emplace_back(change);
}

void SAEVRouteChangelist::emplace_back(SAEVKeyPoint &kp, Bound bound, int value) {
    _changelist.emplace_back(kp, bound, value);
}

void SAEVRouteChangelist::applyChanges() {
    if(_status < InsertionStatus::FAILURE_PRECONDITION_TW) {
        _routePtr->insertRequest(_requestId, _originPredecessorKP, _destinationPredecessorKP);
    }
    for(SAEVRouteChange change : _changelist) {
        change.applyChange();
    }
    _currentStatus = _status;
}

void SAEVRouteChangelist::revertChanges() {
    if(_status < InsertionStatus::FAILURE_PRECONDITION_TW) {
        _routePtr->removeRequest(_requestId);
    }
    for(SAEVRouteChange change : _changelist) {
        change.revertChange();
    }
    _currentStatus = InsertionStatus::CHANGELIST_REVERTED;
}

size_t SAEVRouteChangelist::getRequestId() const {
    return _requestId;
}

const SAEVKeyPoint &SAEVRouteChangelist::getOriginPredecessorKP() const {
    return _originPredecessorKP;
}

const SAEVKeyPoint &SAEVRouteChangelist::getDestinationPredecessorKP() const {
    return _destinationPredecessorKP;
}

double SAEVRouteChangelist::getScore() const {
    return _score;
}

void SAEVRouteChangelist::setScore(double score) {
    SAEVRouteChangelist::_score = score;
}

bool SAEVRouteChangelist::operator>(const SAEVRouteChangelist &rhs) const {
    return _routePtr == rhs.getRoutePtr() && _score > rhs.getScore();
}

bool SAEVRouteChangelist::operator<(const SAEVRouteChangelist &rhs) const {
    return _routePtr == rhs.getRoutePtr() && _score < rhs.getScore();
}

SAEVRoute * SAEVRouteChangelist::getRoutePtr() const {
    return _routePtr;
}

SAEVRouteChangelist::InsertionStatus SAEVRouteChangelist::getStatus() const {
    return _status;
}

SAEVRouteChangelist::InsertionStatus SAEVRouteChangelist::getCurrentStatus() const {
    return _currentStatus;
}

bool SAEVRouteChangelist::shouldUndoInsertion() const {
    return _currentStatus > InsertionStatus::FAILURE_DELTA_MAX;
}

void SAEVRouteChangelist::setStatus(SAEVRouteChangelist::InsertionStatus status) {
    _status = status;
}

void SAEVRouteChangelist::setCurrentStatus(SAEVRouteChangelist::InsertionStatus currentStatus) {
    _currentStatus = currentStatus;
}
