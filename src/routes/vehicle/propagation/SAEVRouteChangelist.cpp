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

void SAEVRouteChangelist::applyChanges() const {
    _routePtr->insertRequest(_requestId, _originPredecessorIdx, _destinationPredecessorIdx);
    for(SAEVRouteChange change : _changelist) {
        change.applyChange();
    }
    //TODO: update key point weights here ? (only if changelist.size > 0 & score < +Inf)
}

void SAEVRouteChangelist::revertChanges() const {
    _routePtr->removeRequest(_requestId);
    for(SAEVRouteChange change : _changelist) {
        change.revertChange();
    }
    //TODO: update key point weights here ? (only if changelist.size > 0 & score < +Inf)
}

size_t SAEVRouteChangelist::getRequestId() const {
    return _requestId;
}

int SAEVRouteChangelist::getOriginPredecessorIdx() const {
    return _originPredecessorIdx;
}

int SAEVRouteChangelist::getDestinationPredecessorIdx() const {
    return _destinationPredecessorIdx;
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
