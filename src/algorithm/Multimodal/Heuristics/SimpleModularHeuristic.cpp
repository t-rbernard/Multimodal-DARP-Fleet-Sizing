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
            if(results.size() == Constants::MAX_TRANSIT_ENTRY_CANDIDATES) {
                break;
            }
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

std::vector<Request>
SimpleModularHeuristic::generateAndInsertBestEntries(const std::vector<Request> &baseRequestsList) {
    std::vector<Request> entrySubRequestsList{baseRequestsList.size()}; //Init entry subrequests list
    for(size_t i = 0; i < baseRequestsList.size(); ++i) {
        const Request& baseRequest = baseRequestsList[i];
        entrySubRequestsList.push_back(insertBestTransitEntryInRoute(baseRequest, i));
    }

    return entrySubRequestsList;
}

Request SimpleModularHeuristic::insertBestTransitEntryInRoute(const Request &baseRequest, size_t requestId) {
    std::vector<Request> entrySubRequestsList{Constants::MAX_TRANSIT_ENTRY_CANDIDATES}; //Init entry subrequests list
    std::vector<TransitAccess> entriesAccessList = getBestTransitEntriesList(baseRequest);
    for(auto const& access : entriesAccessList) {
        entrySubRequestsList.emplace_back(*_graph, baseRequest, access, true);
    }

    for(const auto& subreq : entrySubRequestsList) {
        _route->getEntrySubRequestOrigin(requestId).setRequest(&subreq);
        _route->getEntrySubRequestDestination(requestId).setRequest(&subreq);
        SAEVRouteChangelist changeList = BestInsertionHeuristic::tryBestRequestInsertionInActiveVehicle(_route->getEntrySubRequestOrigin(requestId), *_route);
        if(changeList.success()) {
            //TODO: add subreq to the global requests list here ?
            return subreq;
        }

    }

    //If no active vehicle insertion worked, do best insertion on a new vehicle
    _route->insertRequestInNewVehicle(_route->getEntrySubRequestOrigin(requestId));
    return entrySubRequestsList[0];
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
