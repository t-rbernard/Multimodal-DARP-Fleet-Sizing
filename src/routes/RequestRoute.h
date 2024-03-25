//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_REQUESTROUTE_H
#define GREEDYALGORITHM_REQUESTROUTE_H


#include "RequestKeyPoint.h"

class RequestRoute {
private:
    RequestKeyPoint _route[4];
    Request* _requestPointer{};
public:
    RequestRoute() = default;
    explicit RequestRoute(Request* request) {_requestPointer = request; };

    [[nodiscard]] int getNodeIndex(int routeIndex) const;
    void setNodeIndex(int routeIndex, int nodeIndex);

    [[nodiscard]] int getArrivalInstant(int routeIndex) const;
    void setArrivalInstant(int routeIndex, int arrivalInstant);

    [[nodiscard]] int getDepartureInstant(int routeIndex) const;
    void setDepartureInstant(int routeIndex, int departureInstant);

    [[nodiscard]] SAEVehicle *getSAEV(int routeIndex) const;
    void setSAEV(int routeIndex, SAEVehicle *saev);

    [[nodiscard]] LineStop *getLineStop(int routeIndex) const;
    void setLineStop(int routeIndex, LineStop *lineStop);

    [[nodiscard]] const RequestKeyPoint *getRoute() const;

    [[nodiscard]] Request *getRequestPointer() const;
    void setRequestPointer(Request *requestPointer);

    void resetKeyPoint(int routeIndex);
};


#endif //GREEDYALGORITHM_REQUESTROUTE_H
