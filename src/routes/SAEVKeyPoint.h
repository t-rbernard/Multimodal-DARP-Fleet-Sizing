//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../instance/requests/Request.h"
#include "KeyPoint.h"

class SAEVKeyPoint : public virtual KeyPoint {
private:
    Request* _request;
    bool _isEntry;

public:
    SAEVKeyPoint(Request* request, bool  isEntry);

    [[nodiscard]] Request *getRequest() const;
    void setRequest(Request *request);

    [[nodiscard]] bool isEntry() const;
    void setIsEntry(bool isEntry);

    [[nodiscard]] bool check() const override;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
