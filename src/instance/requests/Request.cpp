//
// Created by rbernard on 22/01/24.
//

#include "Request.h"

Request::Request(const int departureNodeIndex, const int arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const int deltaTime, const int weight) : _departureNodeIndex(departureNodeIndex),
                                                          _arrivalNodeIndex(arrivalNodeIndex), _arrivalTW(arrivalTw),
                                                          _deltaTime(deltaTime), _weight(weight) {
    _currentDeltaTime = deltaTime;
    _departureTW = _arrivalTW - deltaTime;
}
