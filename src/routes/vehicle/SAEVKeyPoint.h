//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVKEYPOINT_H
#define GREEDYALGORITHM_SAEVKEYPOINT_H


#include "../KeyPoint.h"
#include "../../instance/graph/Graph.h"
#include "../../instance/requests/Request.h"

class SAEVKeyPoint : public KeyPoint {
private:
    bool _isOrigin{};
    bool _isDepot{false};
    SAEVKeyPoint* _predecessor{nullptr};
    SAEVKeyPoint* _successor{nullptr};
    SAEVKeyPoint* _counterpart{nullptr};
    int _currentOccupation{0};
    uint _minTW{0};
    uint _maxTW{INT16_MAX};
    SAEVehicle const * _vehiclePointer{};
    Request const * _requestPointer{};
public:
    /**
     * Default KP constructor to allow pre-allocation of our route vector
     */
    SAEVKeyPoint() = default;
    /**
     * Depot KP initialization with [0;max] time windows and a depot node index
     */
    explicit SAEVKeyPoint(int depotNodeIdx);
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

    [[nodiscard]] uint getMinTw() const;
    void setMinTw(uint minTw);

    [[nodiscard]] uint getMaxTw() const;
    void setMaxTw(uint maxTw);

    [[nodiscard]] bool isOrigin() const;
    [[nodiscard]] bool isDestination() const;
    void setIsOrigin(bool isOrigin);

    [[nodiscard]] bool isDepot() const;
    void setIsDepot(bool isDepot);

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

    [[nodiscard]] int getDeltaTime() const;

    [[nodiscard]] std::string to_string() const override;
};


#endif //GREEDYALGORITHM_SAEVKEYPOINT_H