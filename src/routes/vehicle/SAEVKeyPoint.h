//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../../instance/requests/Request.h"
#include "../KeyPoint.h"
#include "../../instance/graph/Graph.h"
#include "propagation/SAEVRouteChange.h"

class SAEVKeyPoint : public KeyPoint {
private:
    bool _isOrigin{};
    SAEVKeyPoint* _predecessor{nullptr};
    SAEVKeyPoint* _successor{nullptr};
    SAEVKeyPoint* _counterpart{nullptr};
    int _currentOccupation{0};
    int _minTW{0};
    int _maxTW{INT16_MAX};
    SAEVehicle const * _vehiclePointer{};
    Request const * _requestPointer{};
public:
    /**
     * Depot KP initialization with [0;max] time windows
     */
    SAEVKeyPoint() = default;
    /**
     * SAEV KP initialization
     * @param graph the instance's graph, used to get shortest path and estimate start time windows relative to distances
     * @param request the request associated with our key point, required to initialize time windows
     * @param isOrigin true iff the key point represents the request's origin, false otherwise
     */
    SAEVKeyPoint(const Graph &graph, const Request& request, bool isOrigin);

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

    [[nodiscard]] bool isOrigin() const;
    void setIsOrigin(bool isOrigin);

    [[nodiscard]] const Request *getRequest() const;
    void setRequest(const Request *requestPointer);

    [[nodiscard]] SAEVKeyPoint *getCounterpart() const;

    void setCounterpart(SAEVKeyPoint *counterpart);

    [[nodiscard]] bool check() const override;

    ~SAEVKeyPoint() override = default;

    /**
     * Two SAEVKeyPoints are equal iff they're the same instance (located at the same adress)
     * It is done this way because a KP doesn't hold the info of the Request O/D it references. Only the route knows this
     * @param rhs right hand side of the comparison
     * @return True iff this and rhs are the same instance at the same adress
     */
    bool operator==(const SAEVKeyPoint &rhs) const;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H
