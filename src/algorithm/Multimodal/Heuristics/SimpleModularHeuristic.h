//
// Created by romain on 15/07/24.
//

#ifndef GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
#define GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H


#include <cstddef>
#include <vector>
#include "../../../instance/requests/Request.h"
#include "../../../routes/vehicle/SAEVRoute.h"
#include "TransitAccess.h"
#include "../../DARP/Heuristics/BestInsertionHeuristic.h"
#include "../../../../test/lib/googletest/googletest/include/gtest/gtest_prod.h"

#ifdef DEBUG_MULTIMODAL_HEURISTIC
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

class SimpleModularHeuristic {
private:
    const Graph* _graph{nullptr};
    SAEVRoute* _route{nullptr};
    std::vector<Request>* _requestsVect{nullptr};
    size_t _nbBaseRquests;

    //Add friend test classes to test inner workings without making the whole API public
    FRIEND_TEST(MultimodalInsertionHeuristicDebug, DebugBaseInstance);
    FRIEND_TEST(MultimodalInsertionHeuristicDebug, DebugInstanceAlain);

//Public interface to interact with the modular heuristic
public:
    SimpleModularHeuristic(const Graph *graph, SAEVRoute *route, std::vector<Request>* requestsVect) : _graph(graph), _route(route),
    _requestsVect(requestsVect), _nbBaseRquests(requestsVect->size()) {}

    void multimodalRequestsInsertion(const std::vector<Request>& requestsToInsert);

    [[nodiscard]] size_t getNbBaseRquests() const {
        return _nbBaseRquests;
    }

//Private members for heuristic internal functions we don't wish to see overriden
private:

    Request insertBestTransitEntryInRoute(const Request &baseRequest, size_t baseRequestId);
    Request insertBestTransitEntryInRoute(const Request &baseRequest, const std::vector<TransitAccess>& entriesAccessList, size_t baseRequestId);
    const Request &insertBestTransitAccessInRoute(const std::vector<Request> &accessSubRequestsList,
                                                  size_t baseRequestId, bool isEntry);

    void insertBestTransitExitsInRoute(const std::vector<Request>& baseRequestsList, const std::vector<TransitAccess>& transitEntriesList);

    //Best candidates function
    /**
     * Creates and returns a vector of TransitAccess objects representing possible
     * transit entries that can be converted to Request objects to try and insert them
     * in a vehicle. <br>
     * <br>
     * This vector is sorted in the same way as Node._bestStationsNodeIdxVector and has max size
     * Constants::MAX_TRANSIT_ENTRY_CANDIDATES. There can be less or even 0 elements if no candidate
     * has a transit departure tmax in getMinEntryConstraint <= tmax <= getMaxEntryConstraint
     *
     * @param baseRequest the request we wish to find potential transit entries for
     * @return A vector consisting of all valid TransitAccess objects wrt the base request
     * and the min/max exit constraints. If no valid access is found, we return an empty vector.
     */
    [[nodiscard]] std::vector<TransitAccess> getBestTransitEntriesList(const Request &baseRequest) const;

    /**
     * Creates and returns a vector of TransitAccess objects representing possible
     * transit entries that can be converted to Request objects to try and insert them
     * in a vehicle. <br>
     * <br>
     * This vector is sorted according to the sort function the class was initialized with
     * (default = t' + T(y,Dr)) and has max size Constants::MAX_TRANSIT_EXIT_CANDIDATES.
     * There can be less, or even 0 elements if no candidate has a transit departure tmax in
     * getMinEntryConstraint <= tmax <= getMaxEntryConstraint
     *
     * @param transitEntryRequest the transit entry sub-request we wish to find potential exits from
     * @return A vector consisting of all valid TransitAccess objects wrt the entry request's state
     * and the min/max entry constraints. If no valid access is found, we return an empty vector.
     */
     std::vector<TransitAccess> getBestTransitExitsList(Request transitEntryRequest);

//Protected member function for overriding as we make this more modular
protected:
    //Entry filter
    [[nodiscard]] uint getMinEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const;
    [[nodiscard]] uint getMaxEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const;
    //Exit filter
    /**
     * Base implementation of min exit constraint.
     * This implementation is equivalent to no min constraint, as our subset of possible moves already filters our options a bit
     * @param baseRequest
     * @param exitNodeIndex
     * @return
     */
    [[nodiscard]] uint getMinExitConstraint(size_t baseRequestId, const TransitAccess &exitData);
    /**
     * Base implementation of max exit constraint.
     * This base implementation just checks for arrival validity wrt the base request's max time window
     * @param baseRequestId Id of the base request for which we generate exit subrequests. Necessary to get data on base request and entry subrequest if necessary
     * @param exitData Data containing the exit node and timestamp
     * @return baseRequest.DestinationTW.max - T(exitData.Node, baseRequest.Destination)
     */
    [[nodiscard]] uint getMaxExitConstraint(size_t baseRequestId, const TransitAccess &exitData);
    /**
     * Base implementation of a sorting score (lower is better) for exit candidates.
     * This implementation scores via T(exitNode, destinationNode) + exitTime to try and
     * incentivize early and/or close access points
     * @param baseRequest the base request, required to get the destination
     * @param exitData exit data containing the exit point and timestamp
     * @return A score allowing to sort transit exits in ascending score order
     */ //TODO: try other scoring functions (current other idea : T(exitNode, destinationNode) * exitTime; alpha*T(exitNode, destinationNode) + exitTime)
    [[nodiscard]] double getTransitExitScore(const Request& baseRequest, const TransitAccess& exitData);

    [[nodiscard]] const Graph *getGraph() const;
    void setGraph(const Graph *graph);

    [[nodiscard]] SAEVRoute *getRoute() const;
    void setRoute(SAEVRoute *route);

    void updateSubRequest(size_t requestId, const Request &request, bool isEntry);

    [[nodiscard]] size_t getSubrequestIndex(size_t requestId, bool isEntry) const;

    const Request &getSubrequest(size_t requestId, bool isEntry);
};


#endif //GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
