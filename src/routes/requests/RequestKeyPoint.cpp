//
// Created by romain on 22/03/24.
//

#include "RequestKeyPoint.h"

SAEVehicle const *RequestKeyPoint::getSAEV() const {
    return _saev;
}

void RequestKeyPoint::setSAEV(const SAEVehicle *saev) {
    _saev = saev;
    _lineStop = nullptr;
}

LineStop const *RequestKeyPoint::getLineStop() const {
    return _lineStop;
}

void RequestKeyPoint::setLineStop(const LineStop *lineStop) {
    _lineStop = lineStop;
    _saev = nullptr;
}

const TimeWindow &RequestKeyPoint::getArrivalTimeWindow() const {
    return _arrivalTimeWindow;
}

void RequestKeyPoint::setArrivalTimeWindow(const TimeWindow &arrivalTimeWindow) {
    _arrivalTimeWindow = arrivalTimeWindow;
}

const TimeWindow &RequestKeyPoint::getDepartureTimeWindow() const {
    return _departureTimeWindow;
}

void RequestKeyPoint::setDepartureTimeWindow(const TimeWindow &departureTimeWindow) {
    _departureTimeWindow = departureTimeWindow;
}


/**
 * Verifies KeyPoint::check() + RequestKeyPoint specific constraints :
 * @see KeyPoint::check()
 * @return True if parent class checks are true and if one of _linestop xor _saev is a nullptr
 */
bool RequestKeyPoint::check() const {
    return KeyPoint::check() && ((_saev != nullptr && _lineStop == nullptr) || (_saev == nullptr && _lineStop != nullptr));
}
