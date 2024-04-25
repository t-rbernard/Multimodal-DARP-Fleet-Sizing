//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_REQUESTROUTE_H
#define GREEDYALGORITHM_REQUESTROUTE_H


#include "RequestKeyPoint.h"

class Request;
class RequestRoute {
public:
    enum RouteStructureEnum {
        SAEV1 = 0,
        TRANSIT1 = 1,
        TRANSIT2 = 2,
        SAEV2 = 3
    };
private:
    RequestKeyPoint _route[4];
    Request* _requestPointer{nullptr};
public:
    RequestRoute() = default;
    explicit RequestRoute(Request* request) {_requestPointer = request; };

    [[nodiscard]] int getNodeIndex(int routeIndex) const;
    void setNodeIndex(int routeIndex, int nodeIndex);

    [[nodiscard]] SAEVehicle *getSAEV(int routeIndex) const;
    void setSAEV(int routeIndex, SAEVehicle *saev);

    [[nodiscard]] LineStop *getLineStop(int routeIndex) const;
    void setLineStop(int routeIndex, LineStop *lineStop);

    [[nodiscard]] const RequestKeyPoint *getRoute() const;

    [[nodiscard]] Request *getRequestPointer() const;
    void setRequestPointer(Request *requestPointer);

    void resetKeyPoint(int routeIndex);
};

#include "../instance/requests/Request.h"

#endif //GREEDYALGORITHM_REQUESTROUTE_H
