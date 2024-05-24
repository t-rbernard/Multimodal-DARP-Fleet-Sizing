//
// Created by rbernard on 22/01/24.
//

#include <charconv>
#include "Request.h"
#include "../../services/DatFile/DATRow.h"

Request::Request(const int departureNodeIndex, const int arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const int deltaTime, const int weight) : _originNodeIndex(departureNodeIndex),
                                                          _destinationNodeIndex(arrivalNodeIndex), _arrivalTW(arrivalTw),
                                                          _deltaTime(deltaTime), _weight(weight) {
    _currentDeltaTime = deltaTime;
    _departureTW = _arrivalTW - deltaTime;
}

Request::Request(const int departureNodeIndex, const int arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const int deltaTime, const int weight, const Graph& graph) :
        _originNodeIndex(departureNodeIndex), _destinationNodeIndex(arrivalNodeIndex),
        _arrivalTW(arrivalTw), _deltaTime(deltaTime), _weight(weight) {
    _currentDeltaTime = deltaTime;
    _departureTW.min = _arrivalTW.min - deltaTime;
    _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(departureNodeIndex, arrivalNodeIndex);
}

Request::Request(const DATRow& currentRow, const Graph& graph) {
    std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].size(), _originNodeIndex);
    std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].size(), _destinationNodeIndex);

    int twMin, twMax;
    std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].size(), twMin);
    std::from_chars(currentRow[3].data(), currentRow[3].data() + currentRow[3].size(), twMax);
    _arrivalTW = TimeWindow(twMin, twMax);

    std::from_chars(currentRow[4].data(), currentRow[4].data() + currentRow[4].size(), _deltaTime);
    std::from_chars(currentRow[5].data(), currentRow[5].data() + currentRow[5].size(), _weight);

    _departureTW.min = _arrivalTW.min - _currentDeltaTime;
    _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex);
}

const int Request::getOriginNodeIndex() const {
    return _originNodeIndex;
}

const int Request::getDestinationNodeIndex() const {
    return _destinationNodeIndex;
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
