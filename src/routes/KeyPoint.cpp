//
// Created by romain on 22/03/24.
//

#include "KeyPoint.h"

KeyPoint::KeyPoint() = default;
KeyPoint::KeyPoint(int nodeIndex, int arrivalInstant, int departureInstant) : _nodeIndex(nodeIndex),
                                                                              _arrivalInstant(arrivalInstant),
                                                                              _departureInstant(departureInstant) {}

int KeyPoint::getNodeIndex() const {
    return _nodeIndex;
}

void KeyPoint::setNodeIndex(int nodeIndex) {
    KeyPoint::_nodeIndex = nodeIndex;
}

int KeyPoint::getArrivalInstant() const {
    return _arrivalInstant;
}

void KeyPoint::setArrivalInstant(int arrivalInstant) {
    KeyPoint::_arrivalInstant = arrivalInstant;
}

int KeyPoint::getDepartureInstant() const {
    return _departureInstant;
}

void KeyPoint::setDepartureInstant(int departureInstant) {
    KeyPoint::_departureInstant = departureInstant;
}

bool KeyPoint::isBeforeKeyPoint(const KeyPoint &kp) const {
    return _departureInstant <= kp.getArrivalInstant();
}

bool KeyPoint::isAfterKeyPoint(const KeyPoint &kp) const {
    return _arrivalInstant >= kp.getDepartureInstant();
}

bool KeyPoint::check() const {
    return _arrivalInstant <= _departureInstant;
}

