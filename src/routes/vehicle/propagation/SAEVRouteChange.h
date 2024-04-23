//
// Created by romain on 23/04/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTECHANGE_H
#define GREEDYALGORITHM_SAEVROUTECHANGE_H

#include "../SAEVKeyPoint.h"

enum Bound {
    Min, Max
};

class SAEVRouteChange {
private:
    SAEVKeyPoint* const _kpPointer;
    const Bound _bound;
    const int _value;
public:
    SAEVRouteChange(SAEVKeyPoint& kp, Bound bound, int value) : _kpPointer(&kp), _bound(bound), _value(value) {};
    /**
     * Unconditionally applies the bound change on the appropriate key point
     * (adds value to the min bound or subtracts value to the max bound)
     */
    void applyChange();
    /**
     * Unconditionally reverts the bound change on the appropriate key point
     * (subtracts value to the min bound or adds value to the max bound)
     */
    void revertChange();
};


#endif //GREEDYALGORITHM_SAEVROUTECHANGE_H
