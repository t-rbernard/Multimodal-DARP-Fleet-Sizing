//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../instance/requests/Request.h"
#include "KeyPoint.h"

class SAEVKeyPoint : KeyPoint {
private:
    Request* _request;
    bool _isEntry;

public:
    [[nodiscard]] Request *getRequest() const;
    void setRequest(Request *request);

    [[nodiscard]] bool isEntry() const;
    void setIsEntry(bool isEntry);
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
