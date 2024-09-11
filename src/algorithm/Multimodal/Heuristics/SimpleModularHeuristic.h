//
// Created by romain on 15/07/24.
//

#ifndef GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
#define GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H


#include <cstddef>
#include <vector>
#include <float.h>
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
public:
    struct ScoredTransitAccess : public TransitAccess {
        double score{DBL_MAX};
        explicit ScoredTransitAccess(const TransitAccess& access, double scr) : TransitAccess(access), score(scr) {}
    };
private:
    const Graph* _graph{nullptr};
    SAEVRoute* _route{nullptr};
    std::vector<Request>* _requestsVect{nullptr};
    size_t _nbBaseRquests;

    //Add friend test classes to test inner workings without making the whole API public
    FRIEND_TEST(MultimodalInsertionHeuristicDebug, DebugBaseInstance);
    FRIEND_TEST(MultimodalInsertionHeuristicDebug, DebugInstanceAlain);


    using transit_order_function = std::function<bool(ScoredTransitAccess, ScoredTransitAccess)>;

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

    const Request & insertBestTransitEntryInRoute(const Request &baseRequest, size_t baseRequestId);
    const Request & insertBestTransitExitsInRoute(const Request &baseRequest, size_t baseRequestId);


    const Request &insertBestTransitAccessInRoute(const Request &baseRequest, const std::vector<TransitAccess> &transitAccessList,
                                   size_t baseRequestId, bool isEntry);
    const Request &insertBestTransitAccessInRoute(const std::vector<Request> &accessSubRequestsList,
                                                  size_t baseRequestId, bool isEntry);

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
     * (default = t' + T(y,Dr)). It does not guarantee a max size of Constants::MAX_TRANSIT_EXIT_CANDIDATES.
     * There can be less, or even 0 elements if no candidate has a transit departure tmax in
     * getMinEntryConstraint <= tmax <= getMaxEntryConstraint
     *
     * @param baseRequestId id of the base request for which we look for best exits
     * @return A vector consisting of all valid TransitAccess objects wrt the entry request's state
     * and the min/max entry constraints. If no valid access is found, we return an empty vector.
     */
    std::vector<TransitAccess> getBestTransitExitsList(size_t baseRequestId);
    /**
    * Creates and returns a vector of TransitAccess objects representing possible
    * transit entries that can be converted to Request objects to try and insert them
    * in a vehicle. <br>
    * <br>
    * This vector is sorted according to the sort function the class was initialized with
    * (default = t' + T(y,Dr)). It does not guarantee a max size of Constants::MAX_TRANSIT_EXIT_CANDIDATES.
    * There can be less, or even 0 elements if no candidate has a transit departure tmax in
    * getMinEntryConstraint <= tmax <= getMaxEntryConstraint
    *
    * @param baseRequestId id of the base request for which we look for best exits
    * @return A vector consisting of all valid TransitAccess objects wrt the entry request's state
    * and the min/max entry constraints. If no valid access is found, we return an empty vector.
    */
    [[nodiscard]] std::vector<TransitAccess>
    getBestTransitExitsList(size_t baseRequestId, const Request &baseRequest,
                            const SAEVKeyPoint &entrySubRequestOriginKP) const;

//Protected member function for overriding as we make this more modular
protected:
    //Entry filter
    [[nodiscard]] uint getMinEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const;
    [[nodiscard]] uint getMaxEntryConstraint(const Request &request, size_t ptEntryNodeIdx) const;
    //Exit filter
    /**
     * Base implementation of min exit constraint.
     * This implementation is equivalent to no min constraint, as our subset of possible moves already filters our options a bit
     * @param baseRequestId Id of the base request for which we generate exit subrequests. Necessary to get data on base request and entry subrequest if necessary
     * @param transitExitNodeIdx exit node index
     * @return 0
     */
    [[nodiscard]] uint getMinExitConstraint(size_t baseRequestId, const SAEVKeyPoint &entrySubRequestOriginKP,
                                            size_t transitExitNodeIdx) const;
    /**
     * Base implementation of max exit constraint.
     * This base implementation just checks for arrival validity wrt the base request's max time window
     * @param baseRequestId Id of the base request for which we generate exit subrequests. Necessary to get data on base request and entry subrequest if necessary
     * @param transitExitNodeIdx exit node index
     * @return baseRequest.DestinationTW.max - T(exitData.Node, baseRequest.Destination)
     */
    [[nodiscard]] uint getMaxExitConstraint(size_t baseRequestId, const SAEVKeyPoint &entrySubRequestOriginKP,
                                            size_t transitExitNodeIdx) const;
    /**
     * Base implementation of a sorting score (lower is better) for exit candidates.
     * This implementation scores via T(exitNode, destinationNode) + exitTime to try and
     * incentivize early and/or close access points. <br> <br>
     * Override this function to alter exit candidates ordering via the score function <br>
     * /!\ Be mindful that vectors are sorted in ascending order, so invert the score or override getScoredTransitExitOrderer if you want greater values first /!\
     * @param baseRequest the base request, required to get the destination
     * @param exitData exit data containing the exit point and timestamp
     * @return A score allowing to sort transit exits in ascending score order
     */ //TODO: try other scoring functions (current other idea : T(exitNode, destinationNode) * exitTime; alpha*T(exitNode, destinationNode) + exitTime)
    [[nodiscard]] double getTransitExitScore(const Request& baseRequest, const TransitAccess& exitData) const;
    /**
     * This function returns an orderer function used to sort the transit exit priority queue. <br> <br>
     * Only override this function if you need a more involved kind of sort function that can't be made by simply overriding getTransitExitScore. <br>
     * /!\ Be mindful that vectors are sorted in ascending order, so invert the order if you want greater values first /!\
     * @return Returns a function taking in argument two ScoredTransitAccess objects and returning a true if lhs is to be ordered before rhs, false otherwise
     */
    [[nodiscard]] static transit_order_function getScoredTransitExitOrderer() ;

    //Keep those getters/setters protected as no other member should access or modify these objects
    [[nodiscard]] const Graph *getGraph() const;
    void setGraph(const Graph *graph);

    [[nodiscard]] SAEVRoute *getRoute() const;
    void setRoute(SAEVRoute *route);

    void updateSubRequest(size_t requestId, const Request &request, bool isEntry);

    [[nodiscard]] size_t getSubrequestIndex(size_t requestId, bool isEntry) const;

    const Request &getSubrequest(size_t requestId, bool isEntry);

    [[nodiscard]] double getTransitExitScore(size_t transitExitNodeIndex, size_t requestDestinationNodeIndex,
                               uint transitExitTimestamp) const;
};


#endif //GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
