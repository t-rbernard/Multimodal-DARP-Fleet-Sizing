//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_REQUEST_H
#define GREEDYALGORITHM_REQUEST_H


#include "../../TimeWindow.h"
#include "../../routes/RequestRoute.h"
#include "../graph/Graph.h"

class Request {
private:
    //Request base members (const and initialized on _request creation)
    int _originNodeIndex; //Starting point of the user _request //TODO (?) change this to a Node pointer eventually
    int _destinationNodeIndex; //
    TimeWindow _arrivalTW; //[min,max] time window for arrival to the destination node
    int _deltaTime; //Base delta time, aka the maximum total duration of the path to serve this _request
    int _weight; //How much space the requests takes in the vehicle (defaults to 1)

    //Request helpful members (used for constraint propagation and remember the current state of the _request)
    int _currentDeltaTime; //deltaTime - currentRouteDuration
    int _requestServiceStart;
    int _requestServiceEnd;
    RequestRoute _currentRoute{this};
    TimeWindow _departureTW; //For now, a virtual TW on departures, used for constraint propagation
public:
    Request(const int departureNodeIndex, const int arrivalNodeIndex,
            const TimeWindow &arrivalTw, const int deltaTime, const int weight);
    Request(const int departureNodeIndex, const int arrivalNodeIndex, const TimeWindow &arrivalTw,
            const int deltaTime, const int weight, const Graph& graph);
    Request(const DATRow& currentRow, const Graph& graph);
    Request(const DATRow& currentRow, double deltaRatio, const Graph& graph);

    static std::vector<Request> getRequestsFromFile(const std::string& datFilePath, const Graph& graph);

    //Getters
    [[nodiscard]] const int getOriginNodeIndex() const;
    [[nodiscard]] const int getDestinationNodeIndex() const;
    [[nodiscard]] const TimeWindow &getArrivalTw() const;
    [[nodiscard]] const int getDeltaTime() const;
    [[nodiscard]] const int getWeight() const;
    [[nodiscard]] int getCurrentDeltaTime() const;
    [[nodiscard]] int getRequestServiceStart() const;
    [[nodiscard]] int getRequestServiceEnd() const;
    [[nodiscard]] const RequestRoute &getCurrentRoute() const;
    [[nodiscard]] const TimeWindow &getDepartureTw() const;
    [[nodiscard]] const int &getMinDepartureTw() const;
    [[nodiscard]] const int &getMaxDepartureTw() const;
    [[nodiscard]] const int &getMinArrivalTw() const;
    [[nodiscard]] const int &getMaxArrivalTw() const;

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

};


#endif //GREEDYALGORITHM_REQUEST_H
