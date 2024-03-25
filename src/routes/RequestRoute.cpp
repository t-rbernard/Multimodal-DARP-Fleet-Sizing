//
// Created by romain on 22/03/24.
//

#include "RequestRoute.h"

int RequestRoute::getNodeIndex(int routeIndex) const {
    return _route[routeIndex].getNodeIndex();
}

void RequestRoute::setNodeIndex(int routeIndex, int nodeIndex) {
    _route[routeIndex].setNodeIndex(nodeIndex);
}

int RequestRoute::getArrivalInstant(int routeIndex) const {
    return _route[routeIndex].getArrivalInstant();
}

void RequestRoute::setArrivalInstant(int routeIndex, int arrivalInstant) {
    _route[routeIndex].setArrivalInstant(arrivalInstant);
}

int RequestRoute::getDepartureInstant(int routeIndex) const {
    return _route[routeIndex].getDepartureInstant();
}

void RequestRoute::setDepartureInstant(int routeIndex, int departureInstant) {
    _route[routeIndex].setDepartureInstant(departureInstant);
}

SAEVehicle *RequestRoute::getSAEV(int routeIndex) const {
    return _route[routeIndex].getSAEV();
}

void RequestRoute::setSAEV(int routeIndex, SAEVehicle *saev) {
    if(routeIndex == 0 || routeIndex == 3)
        _route[routeIndex].setSAEV(saev);

}

LineStop *RequestRoute::getLineStop(int routeIndex) const {
    return _route[routeIndex].getLineStop();
}

void RequestRoute::setLineStop(int routeIndex, LineStop *lineStop) {
    if(routeIndex == 1 || routeIndex == 2)
        _route[routeIndex].setLineStop(lineStop);
}

void RequestRoute::resetKeyPoint(int routeIndex) {
    RequestKeyPoint keyPoint = _route[routeIndex];
    keyPoint.setNodeIndex(-1);
    if(routeIndex > 1) { //If possible, set the time values to be the same as the preceding node to keep _route continuity
        keyPoint.setArrivalInstant(_route[routeIndex - 1].getArrivalInstant());
        keyPoint.setDepartureInstant(keyPoint.getArrivalInstant());
    } else { //otherwise use default values
        keyPoint.setArrivalInstant(-1);
        keyPoint.setDepartureInstant(-1);
    }
    keyPoint.setSAEV(nullptr);
    keyPoint.setLineStop(nullptr);
}

const RequestKeyPoint *RequestRoute::getRoute() const {
    return _route;
}

Request *RequestRoute::getRequestPointer() const {
    return _requestPointer;
}

void RequestRoute::setRequestPointer(Request *requestPointer) {
    _requestPointer = requestPointer;
}
