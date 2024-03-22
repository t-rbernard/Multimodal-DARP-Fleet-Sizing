//
// Created by romain on 22/03/24.
//

#include "RequestKeyPoint.h"

SAEVehicle *RequestKeyPoint::getSAEV() const {
    return _saev;
}

void RequestKeyPoint::setSAEV(SAEVehicle *saev) {
    _saev = saev;
    _lineStop = nullptr;
}

LineStop *RequestKeyPoint::getLineStop() const {
    return _lineStop;
}

void RequestKeyPoint::setLineStop(LineStop *lineStop) {
    _lineStop = lineStop;
    _saev = nullptr;
}
