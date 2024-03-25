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

/**
 * Verifies KeyPoint::check() + RequestKeyPoint specific constraints :
 * @see KeyPoint::check()
 * @return True if parent class checks are true and if one of _linestop xor _saev is a nullptr
 */
bool RequestKeyPoint::check() const {
    return KeyPoint::check() && ((_saev != nullptr && _lineStop == nullptr) || (_saev == nullptr && _lineStop != nullptr));
}
