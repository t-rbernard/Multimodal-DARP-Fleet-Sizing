//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../../instance/requests/Request.h"
#include "../KeyPoint.h"
#include "../../instance/graph/Graph.h"

class SAEVKeyPoint : public KeyPoint {
private:
    SAEVKeyPoint* _predecessor{nullptr};
    SAEVKeyPoint* _successor{nullptr};
    int _currentOccupation{0};
    int _minTW{0};
    int _maxTW{INT16_MAX};
    SAEVehicle* _vehiclePointer{};
public:
    /**
     * Depot KP initialization with [0;max] time windows
     */
    SAEVKeyPoint() = default;
    /**
     * SAEV KP initialization
     * @param graph the instance's graph, used to get shortest path and estimate start time windows relative to distances
     * @param request the request associated with our key point, required to initialize time windows
     * @param isEntry true iff the key point represents the request's origin, false otherwise
     */
    SAEVKeyPoint(const Graph &graph, const Request& request, bool isEntry);

    [[nodiscard]] SAEVKeyPoint *getPredecessor() const;
    void setPredecessor(SAEVKeyPoint *predecessor);

    [[nodiscard]] SAEVKeyPoint *getSuccessor() const;
    void setSuccessor(SAEVKeyPoint *successor);

    [[nodiscard]] int getCurrentOccupation() const;
    void setCurrentOccupation(int currentCapacity);

    [[nodiscard]] int getMinTw() const;
    void setMinTw(int minTw);

    [[nodiscard]] int getMaxTw() const;
    void setMaxTw(int maxTw);

    [[nodiscard]] bool check() const override;

    ~SAEVKeyPoint() override = default;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
