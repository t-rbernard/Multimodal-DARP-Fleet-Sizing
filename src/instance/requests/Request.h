//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_REQUEST_H
#define GREEDYALGORITHM_REQUEST_H


#include "../../TimeWindow.h"
#include "../../routes/requests/RequestRoute.h"
#include "../graph/Graph.h"
#include "../../algorithm/Multimodal/Heuristics/TransitAccess.h"
#include "../../utils/Constants.h"

class Request {
private:
    //Request base members (const and initialized on _request creation)
    size_t _originNodeIndex; //Starting point of the user _request //TODO (?) change this to a Node pointer eventually
    size_t _destinationNodeIndex; //
    TimeWindow _departureTW;
    TimeWindow _arrivalTW; //[min,max] time window for arrival to the destination node
    uint _deltaTime; //Base delta time, aka the maximum total duration of the path to serve this _request
    uint _weight; //How much space the requests takes in the vehicle (defaults to 1)

    //Request helpful members (used for constraint propagation and remember the current state of the _request)
    uint _currentDeltaTime; //deltaTime - currentRouteDuration
    /**
     * Store the timestamp when this request effectively starts its route here once we planified its route in detail
     */
    uint _requestServiceStart;
    /**
     * Store the timestamp when this request effectively ends its route here once we planified its route in detail
     */
    uint _requestServiceEnd;
    /**
     * This object divides our request's route in four key points for each step in the user's path:<br>
     * SAEV departure -> Transit entrance -> Transit exit -> SAEV end <br>
     * Through this object, we'll save the request's path during resolution
     */
    RequestRoute _currentRoute{this};
    /**
     * A ratio by which to multiply the direct vehicle path to guesstimate an upper bound on transit travel time
     */
    double _transitTravelTimeRatio{Constants::BASE_TRANSIT_TRAVEL_TIME_RATIO};
public:
    Request() = default;
    Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex,
            const TimeWindow &arrivalTw, const uint deltaTime, const uint weight);
    Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
            const uint deltaTime, const uint weight, const Graph& graph);
    Request(const DATRow& currentRow, const Graph& graph);
    Request(const DATRow& currentRow, double deltaRatio, const Graph& graph);
    Request(const Graph& graph, const Request &baseRequest, const TransitAccess &access, bool isEntry);

    static std::vector<Request> getRequestsFromFile(const std::string& datFilePath, const Graph& graph);

    //Getters
    [[nodiscard]] size_t getOriginNodeIndex() const;
    [[nodiscard]] size_t getDestinationNodeIndex() const;
    [[nodiscard]] const TimeWindow &getArrivalTw() const;
    [[nodiscard]] uint getDeltaTime() const;
    [[nodiscard]] uint getWeight() const;
    [[nodiscard]] uint getCurrentDeltaTime() const;
    [[nodiscard]] uint getRequestServiceStart() const;
    [[nodiscard]] uint getRequestServiceEnd() const;
    [[nodiscard]] const RequestRoute &getCurrentRoute() const;
    [[nodiscard]] const TimeWindow &getDepartureTw() const;
    [[nodiscard]] uint getMinDepartureTw() const;
    [[nodiscard]] uint getMaxDepartureTw() const;
    [[nodiscard]] uint getMinArrivalTw() const;
    [[nodiscard]] uint getMaxArrivalTw() const;

    //Setters
    void setCurrentDeltaTime(int currentDeltaTime);
    void setRequestServiceStart(int requestServiceStart);
    void setRequestServiceEnd(int requestServiceEnd);
    void setCurrentRoute(const RequestRoute &currentRoute);
    void setDepartureTw(const TimeWindow &departureTw);

    //Route API delegation
    [[nodiscard]] int getNodeIndex(int routeIndex) const;
    void setNodeIndex(int routeIndex, int nodeIndex);

    [[nodiscard]] int getArrivalInstant(int routeIndex) const;
    void setArrivalInstant(int routeIndex, int arrivalInstant);

    [[nodiscard]] int getDepartureInstant(int routeIndex) const;
    void setDepartureInstant(int routeIndex, int departureInstant);

    [[nodiscard]] const SAEVehicle *getSAEV(int routeIndex) const;
    void setSAEV(int routeIndex, SAEVehicle *saev);

    [[nodiscard]] const LineStop *getLineStop(int routeIndex) const;
    void setLineStop(int routeIndex, LineStop *lineStop);

    void resetKeyPoint(int routeIndex);

    [[nodiscard]] double getTransitTravelTimeRatio() const;

    void setTransitTravelTimeRatio(double transitTravelTimeRatio);

    [[nodiscard]] static double computeTransitTravelTimeRatio(double deltaRatio, double travelTimeRatio);
    [[nodiscard]] double computeTransitTravelTimeRatio(const Graph &graph, double travelTimeRatio) const;

    /**
     * Creates a string in an appropriate format for the request to be exported to a file that can be imported again
     * <br> <br>
     * Format : origin_idx,destination_idx,min,max,delta,capacity
     * @return A properly formatted string to import back again
     */
    [[nodiscard]] std::string to_string_export() const;
};


#endif //GREEDYALGORITHM_REQUEST_H
