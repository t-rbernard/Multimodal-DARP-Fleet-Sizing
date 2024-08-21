//
// Created by romain on 15/07/24.
//

#include "SimpleModularHeuristic.h"

std::vector<TransitAccess> SimpleModularHeuristic::getBestTransitEntriesList(const Request &baseRequest) {
    const auto& bestStationsIndexVector = _graph->getNode(
            baseRequest.getOriginNodeIndex()).getBestStationsNodeIdxVector();
    std::vector<TransitAccess> results{bestStationsIndexVector.size()}; //init results vector to the appropriate size
    //Iterate over the best stations saved prior
    for(const auto& bestStationNodeIdx : bestStationsIndexVector) {
        int maxDepartureTime = -1;

        //Iterate over the lines available on the node selected
        for(const Node& bestStationNode = _graph->getNode(bestStationNodeIdx);
            const auto& lineStop : bestStationNode.getPTLinesSet()) {
            //Find the next passage lower or equal to our max entry time constraint
            auto iterator = lineStop.findNextScheduledPassage(getMaxEntryConstraint(baseRequest, bestStationNodeIdx))--;
            if(iterator != lineStop.getSchedule().end() && *iterator > maxDepartureTime //If we've found a valid time that's superior to our current max time
            && *iterator > getMinEntryConstraint(baseRequest, bestStationNodeIdx)) { //and respects min entry time, replace old value
                maxDepartureTime = *iterator;
            }
        }

        //If we've found a valid max departure time for this station, add it to the list
        if(maxDepartureTime > -1) {
            results.emplace_back(bestStationNodeIdx, maxDepartureTime);
        }
    }
    return results;
}

uint SimpleModularHeuristic::getMinEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const {
    return request.getMinDepartureTw() + _graph->getShortestSAEVPath(request.getOriginNodeIndex(), ptEntryNodeIdx);
}

uint SimpleModularHeuristic::getMaxEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const {
    return (uint) std::floor(request.getMaxArrivalTw() - _graph->getShortestSAEVPath(ptEntryNodeIdx, request.getDestinationNodeIndex()) * request.getTransitTravelTimeRatio());
}

const Graph *SimpleModularHeuristic::getGraph() const {
    return _graph;
}

void SimpleModularHeuristic::setGraph(const Graph *graph) {
    _graph = graph;
}

SAEVRoute *SimpleModularHeuristic::getRoute() const {
    return _route;
}

void SimpleModularHeuristic::setRoute(SAEVRoute *route) {
    _route = route;
}
