//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_REQUEST_H
#define GREEDYALGORITHM_REQUEST_H


#include "../../TimeWindow.h"
#include "../../routes/RequestRoute.h"

class Request {
private:
    //Request base members (const and initialized on _request creation)
    const int _departureNodeIndex; //Starting point of the user _request //TODO (?) change this to a Node pointer eventually
    const int _arrivalNodeIndex; //
    const TimeWindow _arrivalTW; //[min,max] time window for arrival to the destination node
    const int _deltaTime; //Base delta time, aka the maximum total duration of the path to serve this _request
    const int _weight; //How much space the requests takes in the vehicle (defaults to 1)

    //Request helpful members (used for constraint propagation and remember the current state of the _request)
    int _currentDeltaTime; //deltaTime - currentRouteDuration
    int _requestServiceStart;
    int _requestServiceEnd;
    RequestRoute _currentRoute;
    TimeWindow _departureTW; //For now, a virtual TW on departures, used for constraint propagation
public:
    Request(const int departureNodeIndex, const int arrivalNodeIndex,
            const TimeWindow &arrivalTw, const int deltaTime, const int weight);
};


#endif //GREEDYALGORITHM_REQUEST_H
