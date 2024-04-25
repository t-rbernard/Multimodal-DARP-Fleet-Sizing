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

Request::Request(const int departureNodeIndex, const int arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const int deltaTime, const int weight, const Graph& graph) :
                 _departureNodeIndex(departureNodeIndex),_arrivalNodeIndex(arrivalNodeIndex),
                 _arrivalTW(arrivalTw),_deltaTime(deltaTime), _weight(weight) {
    _currentDeltaTime = deltaTime;
    _departureTW.min = _arrivalTW.min - deltaTime;
    _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(departureNodeIndex, arrivalNodeIndex);
}

const int Request::getDepartureNodeIndex() const {
    return _departureNodeIndex;
}

const int Request::getArrivalNodeIndex() const {
    return _arrivalNodeIndex;
}

const TimeWindow &Request::getArrivalTw() const {
    return _arrivalTW;
}

const int Request::getDeltaTime() const {
    return _deltaTime;
}

const int Request::getWeight() const {
    return _weight;
}

int Request::getCurrentDeltaTime() const {
    return _currentDeltaTime;
}

int Request::getRequestServiceStart() const {
    return _requestServiceStart;
}

int Request::getRequestServiceEnd() const {
    return _requestServiceEnd;
}

const RequestRoute &Request::getCurrentRoute() const {
    return _currentRoute;
}

const TimeWindow &Request::getDepartureTw() const {
    return _departureTW;
}

const int &Request::getMinDepartureTw() const {
    return _departureTW.min;
}


const int &Request::getMaxDepartureTw() const {
    return _departureTW.max;
}

const int &Request::getMinArrivalTw() const {
    return _arrivalTW.min;
}


const int &Request::getMaxArrivalTw() const {
    return _arrivalTW.max;
}

//-----------------------------
//---------- Setters ----------
//-----------------------------

void Request::setCurrentDeltaTime(int currentDeltaTime) {
    _currentDeltaTime = currentDeltaTime;
}

void Request::setRequestServiceStart(int requestServiceStart) {
    _requestServiceStart = requestServiceStart;
}

void Request::setRequestServiceEnd(int requestServiceEnd) {
    _requestServiceEnd = requestServiceEnd;
}

void Request::setCurrentRoute(const RequestRoute &currentRoute) {
    _currentRoute = currentRoute;
}

void Request::setDepartureTw(const TimeWindow &departureTw) {
    _departureTW = departureTw;
}

//-----------------------------
//--------- Route API ---------
//-----------------------------

int Request::getNodeIndex(int routeIndex) const {
    return _currentRoute.getNodeIndex(routeIndex);
}

void Request::setNodeIndex(int routeIndex, int nodeIndex) {
    _currentRoute.setNodeIndex(routeIndex, nodeIndex);
}

SAEVehicle *Request::getSAEV(int routeIndex) const {
    return _currentRoute.getSAEV(routeIndex);
}

void Request::setSAEV(int routeIndex, SAEVehicle *saev) {
    _currentRoute.setSAEV(routeIndex, saev);
}

LineStop *Request::getLineStop(int routeIndex) const {
    return _currentRoute.getLineStop(routeIndex);
}

void Request::setLineStop(int routeIndex, LineStop *lineStop) {
    _currentRoute.setLineStop(routeIndex, lineStop);
}

void Request::resetKeyPoint(int routeIndex) {
    _currentRoute.resetKeyPoint(routeIndex);
}
