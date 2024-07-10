//
// Created by romain on 23/04/24.
//

#include "SAEVRouteChange.h"

void SAEVRouteChange::applyChange() {
    if(_bound == Min) {
        _kpPointer->setMinTw(_kpPointer->getMinTw() + _value);
    } else {
        _kpPointer->setMaxTw(_kpPointer->getMaxTw() - _value);
    }
}

void SAEVRouteChange::revertChange() {
    if(_bound == Min) {
        _kpPointer->setMinTw(_kpPointer->getMinTw() - _value);
    } else {
        _kpPointer->setMaxTw(_kpPointer->getMaxTw() + _value);
    }
}
