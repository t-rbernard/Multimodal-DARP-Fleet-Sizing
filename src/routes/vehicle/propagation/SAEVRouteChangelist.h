//
// Created by romain on 23/04/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTECHANGELIST_H
#define GREEDYALGORITHM_SAEVROUTECHANGELIST_H


#include "SAEVRouteChange.h"
#include "../SAEVRoute.h"

class SAEVRouteChangelist {
private:
    const int _requestIdx, _originIdx, _destinationIdx;
    std::vector<SAEVRouteChange> _changelist{};

public:
    explicit SAEVRouteChangelist(const int requestIdx, const int originIdx, const int destinationIdx)
    : _requestIdx(requestIdx), _originIdx(originIdx), _destinationIdx(destinationIdx) {};

    [[nodiscard]] const std::vector<SAEVRouteChange> &getChangelist() const;
    [[nodiscard]] int getRequestIdx() const;
    [[nodiscard]] int getOriginIdx() const;
    [[nodiscard]] int getDestinationIdx() const;

    void push_back(SAEVRouteChange change);
    void emplace_back(SAEVRouteChange change);
    void emplace_back(SAEVKeyPoint& kp, Bound bound, int value);

    void applyChanges(SAEVRoute route) const;
    void revertChanges(SAEVRoute route) const;

};


#endif //GREEDYALGORITHM_SAEVROUTECHANGELIST_H
