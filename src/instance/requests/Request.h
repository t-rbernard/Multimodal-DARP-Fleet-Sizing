//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_REQUEST_H
#define GREEDYALGORITHM_REQUEST_H


#include "../../TimeWindow.h"
#include "../../routes/requests/RequestRoute.h"
#include "../graph/Graph.h"

class Request {
private:
    //Request base members (const and initialized on _request creation)
    size_t _originNodeIndex; //Starting point of the user _request //TODO (?) change this to a Node pointer eventually
    size_t _destinationNodeIndex; //
    TimeWindow _arrivalTW; //[min,max] time window for arrival to the destination node
    uint _deltaTime; //Base delta time, aka the maximum total duration of the path to serve this _request
    uint _weight; //How much space the requests takes in the vehicle (defaults to 1)

    //Request helpful members (used for constraint propagation and remember the current state of the _request)
    uint _currentDeltaTime; //deltaTime - currentRouteDuration
    uint _requestServiceStart;
    uint _requestServiceEnd;
    RequestRoute _currentRoute{this};
    TimeWindow _departureTW; //For now, a virtual TW on departures, used for constraint propagation
public:
    Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex,
            const TimeWindow &arrivalTw, const uint deltaTime, const uint weight);
    Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
            const uint deltaTime, const uint weight, const Graph& graph);
    Request(const DATRow& currentRow, const Graph& graph);
    Request(const DATRow& currentRow, double deltaRatio, const Graph& graph);

    static std::vector<Request> getRequestsFromFile(const std::string& datFilePath, const Graph& graph);

    //Getters
    [[nodiscard]] size_t getOriginNodeIndex() const;
    [[nodiscard]] size_t getDestinationNodeIndex() const;
    [[nodiscard]] const TimeWindow &getArrivalTw() const;
    [[nodiscard]] int getDeltaTime() const;
    [[nodiscard]] int getWeight() const;
    [[nodiscard]] int getCurrentDeltaTime() const;
    [[nodiscard]] int getRequestServiceStart() const;
    [[nodiscard]] int getRequestServiceEnd() const;
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

    /**
     * Creates a string in an appropriate format for the request to be exported to a file that can be imported again
     * <br> <br>
     * Format : origin_idx,destination_idx,min,max,delta,capacity
     * @return A properly formatted string to import back again
     */
    std::string to_string_export();
};


#endif //GREEDYALGORITHM_REQUEST_H
