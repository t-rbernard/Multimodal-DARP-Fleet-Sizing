//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"

class SAEVRoute {
private:
    std::vector<SAEVKeyPoint> _route;
    size_t _nbRequest;
public:
    SAEVRoute() = default;
    explicit SAEVRoute(const std::vector<Request>& requestList);
    void initSAEVRoute(const std::vector<Request>& requestList);

    [[nodiscard]] const std::vector<SAEVKeyPoint> &getRoute() const {
        return _route;
    }


};


#endif //GREEDYALGORITHM_SAEVROUTE_H
