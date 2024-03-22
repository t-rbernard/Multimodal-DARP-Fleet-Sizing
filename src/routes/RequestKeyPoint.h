//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_REQUESTKEYPOINT_H
#define GREEDYALGORITHM_REQUESTKEYPOINT_H


#include "KeyPoint.h"
#include "../instance/SAEVehicle.h"
#include "../instance/graph/LineStop.h"

class RequestKeyPoint : KeyPoint {
private:
    SAEVehicle* _saev;
    LineStop* _lineStop;

public:
    RequestKeyPoint() = default;
    explicit RequestKeyPoint(SAEVehicle* vehicle) { _saev = vehicle; }

    [[nodiscard]] SAEVehicle *getSAEV() const;
    void setSAEV(SAEVehicle *saev);

    [[nodiscard]] LineStop *getLineStop() const;
    void setLineStop(LineStop *lineStop);
};


#endif //GREEDYALGORITHM_REQUESTKEYPOINT_H
