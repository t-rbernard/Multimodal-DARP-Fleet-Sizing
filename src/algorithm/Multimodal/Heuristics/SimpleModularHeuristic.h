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

    Request insertBestTransitEntryInRoute(const Request &baseRequest, size_t requestId);
    Request insertBestTransitEntryInRoute(const Request &baseRequest, const std::vector<TransitAccess>& entriesAccessList, size_t requestId);
    const Request & insertBestTransitEntryInRoute(const std::vector<Request>& entrySubRequestsList, size_t requestId);

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
    uint getMinExitConstraint(const Request &request, size_t exitNodeIndex);
    uint getMaxExitConstraint(const Request &request, size_t exitNodeIndex);

    [[nodiscard]] const Graph *getGraph() const;
    void setGraph(const Graph *graph);

    [[nodiscard]] SAEVRoute *getRoute() const;
    void setRoute(SAEVRoute *route);

    void updateSubRequest(size_t requestId, const Request &request, bool isEntry);

    size_t getSubrequestIndex(size_t requestId, bool isEntry) const;

    const Request &getSubrequest(size_t requestId, bool isEntry);
};


#endif //GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
