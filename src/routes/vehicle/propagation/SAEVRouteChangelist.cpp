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

void SAEVRouteChangelist::applyChanges(SAEVRoute route) const {
    route.insertRequest(_requestIdx, _originIdx, _destinationIdx);
    for(SAEVRouteChange change : _changelist) {
        change.applyChange();
    }
}

void SAEVRouteChangelist::revertChanges(SAEVRoute route) const {
    route.removeRequest(_requestIdx);
    for(SAEVRouteChange change : _changelist) {
        change.revertChange();
    }
}

int SAEVRouteChangelist::getRequestIdx() const {
    return _requestIdx;
}

int SAEVRouteChangelist::getOriginIdx() const {
    return _originIdx;
}

int SAEVRouteChangelist::getDestinationIdx() const {
    return _destinationIdx;
}

