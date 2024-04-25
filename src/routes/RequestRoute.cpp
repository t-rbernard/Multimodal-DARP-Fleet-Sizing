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

SAEVehicle *RequestRoute::getSAEV(int routeIndex) const {
    return _route[routeIndex].getSAEV();
}

void RequestRoute::setSAEV(int routeIndex, SAEVehicle *saev) {
    if(routeIndex == SAEV1 || routeIndex == SAEV2)
        _route[routeIndex].setSAEV(saev);

}

LineStop *RequestRoute::getLineStop(int routeIndex) const {
    return _route[routeIndex].getLineStop();
}

void RequestRoute::setLineStop(int routeIndex, LineStop *lineStop) {
    if(routeIndex == TRANSIT1 || routeIndex == TRANSIT2)
        _route[routeIndex].setLineStop(lineStop);
}

void RequestRoute::resetKeyPoint(int routeIndex) {
    RequestKeyPoint keyPoint = _route[routeIndex];
    keyPoint.setNodeIndex(-1);
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
