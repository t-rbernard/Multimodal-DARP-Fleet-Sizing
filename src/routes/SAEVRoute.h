//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"

class SAEVRoute {
private:
    std::vector<SAEVKeyPoint> route;
public:
    void emplaceBack(SAEVKeyPoint keyPoint) { route.emplace_back(keyPoint); }
};


#endif //GREEDYALGORITHM_SAEVROUTE_H
