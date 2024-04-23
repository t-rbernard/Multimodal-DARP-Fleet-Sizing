//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../../instance/requests/Request.h"
#include "../KeyPoint.h"
#include "../../instance/graph/Graph.h"

class SAEVKeyPoint : public virtual KeyPoint {
private:
    SAEVKeyPoint* _predecessor{nullptr};
    SAEVKeyPoint* _successor{nullptr};
    int _currentOccupation{0};
    int _minTW{0};
    int _maxTW{0};
    SAEVehicle* _vehiclePointer{};

public:
    /**
     * Depot KP initialization
     * @param isEntry
     */
    SAEVKeyPoint(bool isEntry);
    /**
     * SAEV KP initialization
     * @param graph
     * @param request
     * @param isEntry
     */
    SAEVKeyPoint(const Graph &graph, const Request& request, bool isEntry);

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

    [[nodiscard]] bool check() const override;

    ~SAEVKeyPoint() = default;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
