//
// Created by romain on 15/07/24.
//

#include "SimpleModularHeuristic.h"

std::vector<TransitAccess> SimpleModularHeuristic::getBestTransitEntriesList(const Request &baseRequest) const {
    const auto& bestStationsIndexVector = _graph->getNode(
            baseRequest.getOriginNodeIndex()).getBestStationsNodeIdxVector();
    std::vector<TransitAccess> results; //init results vector to the appropriate size
    results.reserve(std::min(Constants::MAX_TRANSIT_ENTRY_CANDIDATES, bestStationsIndexVector.size()));
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

/**
 * Generates the best entries list (ordered by an implementation-dependant ,
 * then generates a vector of requests from these entries before starting the entry subrequest insertion process <br>
 * The insertion process first tries best insertions without creating a new vehicles in order of the best entries list.
 * If none result in a valid insertion, we insert the first subrequest (supposedly the better one) in a new vehicle
 * @param baseRequest const ref to the request we use as our base to get the best entries list
 * @param baseRequestId ID/index in the request vector for our base request
 * @return The subrequest successfully inserted in our route. This method's caller needs to add this request to its main request vector
 */
Request SimpleModularHeuristic::insertBestTransitEntryInRoute(const Request &baseRequest, size_t baseRequestId) {
    std::vector<TransitAccess> entriesAccessList = getBestTransitEntriesList(baseRequest);
    return insertBestTransitEntryInRoute(baseRequest, entriesAccessList, baseRequestId);
}

/**
 * Generates a vector of requests from the given entries list before starting the entry subrequest insertion process <br>
 * The insertion process first tries best insertions without creating a new vehicles in order of the best entries list.
 * If none result in a valid insertion, we insert the first subrequest (supposedly the better one) in a new vehicle
 * @param baseRequest const ref to the request we use as our base to generate subrequest frm the best entries list
 * @param entriesAccessList A list of best entries, preferably ordered from best to worst and sized according to the max number of candidates we want to try inserting
 * @param baseRequestId ID/index in the request vector for our base request
 * @return The subrequest successfully inserted in our route. This method's caller needs to add this request to its main request vector
 */
Request SimpleModularHeuristic::insertBestTransitEntryInRoute(const Request &baseRequest, const std::vector<TransitAccess>& entriesAccessList, size_t baseRequestId) {
    std::vector<Request> entrySubRequestsList;
    entrySubRequestsList.reserve(Constants::MAX_TRANSIT_ENTRY_CANDIDATES); //Init entry subrequests list to the appropriate size
    //Generate subrequests from best transit entries
    for(auto const& access : entriesAccessList) {
        entrySubRequestsList.emplace_back(*_graph, baseRequest, access, true);
    }
    return insertBestTransitAccessInRoute(entrySubRequestsList, baseRequestId, true);
}

/**
 * The insertion process first tries best insertions without creating a new vehicles in order of the best entries list.
 * If none result in a valid insertion, we insert the first subrequest (supposedly the better one) in a new vehicle
 * @param accessSubRequestsList A list of entry subrequest candidates, preferably ordered from best to worst candidate, but the list order is implementation dependant
 * @param baseRequestId ID/index in the request vector for our base request
 * @param isEntry true iff the given access requests are transit entry requests
 * @return The subrequest successfully inserted in our route. This method's caller needs to add this request to its main request vector
 */
const Request &
SimpleModularHeuristic::insertBestTransitAccessInRoute(const std::vector<Request> &accessSubRequestsList,
                                                       size_t baseRequestId, bool isEntry) {
    for(const auto& subreq : accessSubRequestsList) {
        updateSubRequest(baseRequestId, subreq, isEntry);
        SAEVRouteChangelist changeList = BestInsertionHeuristic::tryBestRequestInsertionInActiveVehicle(_route->getEntrySubRequestOrigin(baseRequestId), *_route);
        //If we've found an insertion that doesn't create a vehicle, stop there
        if(changeList.success()) {
            DEBUG_MSG("ENTRY CANDIDATE SUCCESS : " + subreq.to_string());
            return getSubrequest(baseRequestId, isEntry);
        } else {
            DEBUG_MSG("ENTRY CANDIDATE FAILURE : " + subreq.to_string());
        }
    }

    //If no active vehicle insertion worked, do best insertion on a new vehicle with the first subrequest (supposedly it's the most advantageous)
    DEBUG_MSG("CREATE VEHICLE");
    updateSubRequest(baseRequestId, accessSubRequestsList[0], isEntry);
    _route->insertRequestInNewVehicle(_route->getEntrySubRequestOrigin(baseRequestId));
    return getSubrequest(baseRequestId, isEntry);
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

void SimpleModularHeuristic::updateSubRequest(size_t requestId, const Request &request, bool isEntry) {
    size_t subRequestIndex = getSubrequestIndex(requestId, isEntry);
    if((_requestsVect->size() - 1) < subRequestIndex) {
        _requestsVect->emplace_back(request);
        assertm((_requestsVect->size() - 1) == subRequestIndex,"A request seems to have been missed or doesn't have any subrequest defined");
    } else {
        (*_requestsVect)[subRequestIndex] = request;
    }

    std::vector<Request>& requestVectRef = *_requestsVect;
    _route->getEntrySubRequestOrigin(requestId).setRequest(&requestVectRef[subRequestIndex]);
    _route->getEntrySubRequestDestination(requestId).setRequest(&requestVectRef[subRequestIndex]);
}

size_t SimpleModularHeuristic::getSubrequestIndex(size_t requestId, bool isEntry) const {
    if(isEntry)
        return _nbBaseRquests + requestId;
    else
        return _nbBaseRquests*2 + requestId;
}

const Request& SimpleModularHeuristic::getSubrequest(size_t requestId, bool isEntry) {
    return (*_requestsVect)[getSubrequestIndex(requestId, isEntry)];
}

double SimpleModularHeuristic::getTransitExitScore(const Request &baseRequest, const TransitAccess &exitData) {
    return _graph->getShortestSAEVPath(exitData.getAccessNodeIdx(), baseRequest.getDestinationNodeIndex())
           + exitData.getAccessTimestamp();
}

uint SimpleModularHeuristic::getMinExitConstraint(size_t baseRequestId, const TransitAccess &exitData) {
    return 0;
}

uint SimpleModularHeuristic::getMaxExitConstraint(size_t baseRequestId, const TransitAccess &exitData) {
    const Request& baseRequest = (*_requestsVect)[baseRequestId];
    return baseRequest.getMaxArrivalTw() - _graph->getShortestSAEVPath(exitData.getAccessNodeIdx(), baseRequest.getDestinationNodeIndex());
}
