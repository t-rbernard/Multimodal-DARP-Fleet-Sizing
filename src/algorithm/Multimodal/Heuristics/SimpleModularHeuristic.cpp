//
// Created by romain on 15/07/24.
//

#include "SimpleModularHeuristic.h"

using transit_order_function = std::function<bool(SimpleModularHeuristic::ScoredTransitAccess, SimpleModularHeuristic::ScoredTransitAccess)>;

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
            auto iterator = --lineStop.findNextScheduledPassage(getMaxEntryConstraint(baseRequest, bestStationNodeIdx))--;
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
const Request & SimpleModularHeuristic::insertBestTransitEntryInRoute(const Request &baseRequest, size_t baseRequestId) {
    std::vector<TransitAccess> entriesAccessList = getBestTransitEntriesList(baseRequest);
    return insertBestTransitAccessInRoute(baseRequest, entriesAccessList, baseRequestId, true);
}

/**
 * Generates a vector of requests from the given access list before starting the entry subrequest insertion process <br>
 * The insertion process first tries best insertions without creating a new vehicles in order of the best entries list.
 * If none result in a valid insertion, we insert the first subrequest (supposedly the better one) in a new vehicle
 * @param baseRequest const ref to the request we use as our base to generate subrequests from the sorted best access list
 * @param transitAccessList A list of best entries, preferably ordered from best to worst and sized according to the max number of candidates we want to try inserting
 * @param baseRequestId ID/index in the request vector for our base request
 * @return The subrequest successfully inserted in our route. This method's caller needs to add this request to its main request vector
 */
const Request &
SimpleModularHeuristic::insertBestTransitAccessInRoute(const Request &baseRequest,
                                                       const std::vector<TransitAccess> &transitAccessList,
                                                       size_t baseRequestId, bool isEntry) {
    std::vector<Request> accessSubRequestsList;
    size_t maxSize = isEntry ? Constants::MAX_TRANSIT_ENTRY_CANDIDATES : Constants::MAX_TRANSIT_EXIT_CANDIDATES;
    accessSubRequestsList.reserve(maxSize); //Init entry subrequests list to the appropriate size
    //Generate subrequests from best transit entries
    for(auto const& access : transitAccessList) {
        try {
            if(isEntry)
                accessSubRequestsList.emplace_back(*_graph, baseRequest, access);
            else
                accessSubRequestsList.emplace_back(*_graph, baseRequest, access,
                                                   _route->getEntrySubRequestOrigin(baseRequestId));
        }
        catch(const TimeWindow::invalid_time_window_exception& e) {
            DEBUG_MSG("Invalid Time Window during candidate sub request creation, it won't be added to the list");
        }
    }
    return insertBestTransitAccessInRoute(accessSubRequestsList, baseRequestId, isEntry);
}

/**
 * The insertion process first tries best insertions without creating a new vehicles in order of the best entries list. <br>
 * If none result in a valid insertion, we insert the first subrequest (supposedly the better one) in a new vehicle. <br>
 * While doing these insertions, the route and global request vector is updated with the appropriate request so that no inconsistencies in data structures happen
 * @param accessSubRequestsList A list of entry subrequest candidates, preferably ordered from best to worst candidate, but the list order is implementation dependant
 * @param baseRequestId ID/index in the request vector for our base request
 * @param isEntry true iff the given access requests are transit entry requests
 * @return The subrequest successfully inserted in our route
 */
const Request &
SimpleModularHeuristic::insertBestTransitAccessInRoute(const std::vector<Request> &accessSubRequestsList,
                                                       size_t baseRequestId, bool isEntry) {
    for(const auto& subreq : accessSubRequestsList) {
        updateSubRequest(baseRequestId, subreq, isEntry);
        SAEVRouteChangelist changeList = BestInsertionHeuristic::tryBestRequestInsertionInActiveVehicle(
                _route->getEntrySubRequestOrigin(baseRequestId), *_route);
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

double SimpleModularHeuristic::getTransitExitScore(const Request &baseRequest, const TransitAccess &exitData) const {
    return getTransitExitScore(exitData.getAccessNodeIdx(), baseRequest.getDestinationNodeIndex(), exitData.getAccessTimestamp());
}

double SimpleModularHeuristic::getTransitExitScore(size_t transitExitNodeIndex, size_t requestDestinationNodeIndex, uint transitExitTimestamp) const {
    return _graph->getShortestSAEVPath(transitExitNodeIndex, requestDestinationNodeIndex) + transitExitTimestamp;
}

transit_order_function
SimpleModularHeuristic::getScoredTransitExitOrderer() {
    return [](SimpleModularHeuristic::ScoredTransitAccess lhs, SimpleModularHeuristic::ScoredTransitAccess rhs) { return lhs.score < rhs.score; };
}

uint SimpleModularHeuristic::getMinExitConstraint(size_t baseRequestId, const SAEVKeyPoint &entrySubRequestOriginKP,
                                                  size_t transitExitNodeIdx) const {
    return 0;
}

uint SimpleModularHeuristic::getMaxExitConstraint(size_t baseRequestId, const SAEVKeyPoint &entrySubRequestOriginKP,
                                                  size_t transitExitNodeIdx) const {
    const Request& baseRequest = (*_requestsVect)[baseRequestId];
    return (entrySubRequestOriginKP.getMinTw() + baseRequest.getDeltaTime()) -
           _graph->getShortestSAEVPath(transitExitNodeIdx, baseRequest.getDestinationNodeIndex());
}

std::vector<TransitAccess>
SimpleModularHeuristic::getBestTransitExitsList(size_t baseRequestId) {
    const Request& baseRequest = (*_requestsVect)[baseRequestId];
    const SAEVKeyPoint& entrySubRequestOriginKP = _route->getEntrySubRequestOrigin(baseRequestId);
    return getBestTransitExitsList(baseRequestId, baseRequest, entrySubRequestOriginKP);
}

std::vector<TransitAccess>
SimpleModularHeuristic::getBestTransitExitsList(size_t baseRequestId, const Request &baseRequest,
                                                const SAEVKeyPoint &entrySubRequestOriginKP) const {
    std::vector<SimpleModularHeuristic::ScoredTransitAccess> scoredTransitExits;
    //Get departure time/shortest transit paths list from the entry sub request's max time (this means we take the first transit available after current max arrival)
    //TODO : study other approaches (e.g check for a faster max arrival if it's valid and allows better paths. This would require propagation => costly)
    const auto& [departureTime, shortestTransitPaths] = _graph->getShortestTransitPathsFrom(entrySubRequestOriginKP.getCounterpart()->getNodeIndex(),
                                                                   entrySubRequestOriginKP.getCounterpart()->getMaxTw());

    //Iterate over the best stations saved prior
    for(const auto& shortestTransitPath : shortestTransitPaths) {
        //Check valid transit path + arrival node != starting point
        if(shortestTransitPath.getArrivalTime() >= 0 && shortestTransitPath.getArrivalNode() != entryNodeIdx
        && shortestTransitPath.getArrivalTime() < getMaxExitConstraint(baseRequestId, entrySubRequestOriginKP, shortestTransitPath.getArrivalNode())) {
            TransitAccess exit{shortestTransitPath.getArrivalNode(),  (uint) shortestTransitPath.getArrivalTime()};
            scoredTransitExits.emplace_back(exit, getTransitExitScore(baseRequest, exit));
        }
    }

    //Sort and truncate transit exits list while removing score data that's unnecessary in later steps
    std::ranges::sort(scoredTransitExits, getScoredTransitExitOrderer());
    uint finalVectorSize = std::min(scoredTransitExits.size(), Constants::MAX_TRANSIT_EXIT_CANDIDATES);
    std::vector<TransitAccess> truncatedTransitExitsList{scoredTransitExits.begin(), scoredTransitExits.begin() + finalVectorSize};
    return truncatedTransitExitsList;
}

const Request & SimpleModularHeuristic::insertBestTransitExitsInRoute(const Request &baseRequest, size_t baseRequestId) {
    std::vector<TransitAccess> exitAccessList = getBestTransitExitsList(baseRequestId);
    return insertBestTransitAccessInRoute(baseRequest, exitAccessList, baseRequestId, false);
}
