//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../instance/requests/Request.h"
#include "KeyPoint.h"

class SAEVKeyPoint : public virtual KeyPoint {
private:
    Request* _request{};
    SAEVKeyPoint* _predecessor{};
    SAEVKeyPoint* _successor{};
    int _currentCapacity{0};
    int _minTW{0};
    int _maxTW{0};
    SAEVehicle* _vehiclePointer{};

public:
    [[nodiscard]] SAEVKeyPoint *getPredecessor() const;
    void setPredecessor(SAEVKeyPoint *predecessor);

    [[nodiscard]] SAEVKeyPoint *getSuccessor() const;
    void setSuccessor(SAEVKeyPoint *successor);

    [[nodiscard]] int getCurrentCapacity() const;
    void setCurrentCapacity(int currentCapacity);

    [[nodiscard]] int getMinTw() const;
    void setMinTw(int minTw);

    [[nodiscard]] int getMaxTw() const;
    void setMaxTw(int maxTw);

public:
    SAEVKeyPoint(Request* request, bool  isEntry);

    [[nodiscard]] Request *getRequest() const;
    void setRequest(Request *request);

    [[nodiscard]] bool check() const override;

    ~SAEVKeyPoint() = default;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
