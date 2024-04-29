//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"

class SAEVRouteChangelist;
class SAEVRoute {
private:
    std::vector<SAEVKeyPoint> _route;
    size_t _nbRequest;
    const Graph* _graph;
    const std::vector<Request>* _requestList;
public:
    SAEVRoute() = default;
    /**
     * Initializes a route following a tabular doubly chained approach where 0->nbRequest*2-1 are Request Origin (even)/Destination (odd)
     * and nbRequest*2 -> nbRequest*4-1 are vehicle start (even)/end (odd) depot. Each KeyPoint has
     * @param graph the instance's graph, used to get shortest paths values during key point initialization and later in constraint propagation
     * @param requestList the instance's request list, used to associate KeyPoints that represent requests with the appropriate informations
     */
    explicit SAEVRoute(const Graph& graph, const std::vector<Request>& requestList);

    [[nodiscard]] const std::vector<SAEVKeyPoint> &getRoute() const {
        return _route;
    }

    /**
     * Raw request route insertion method. Public for debug purposes, but should effectively never be called by an outside member
     * @param requestIdx index of the request we want to insert in the route
     * @param originRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede the origin of the request we want to insert
     * @param destinationRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede the destination of the request we want to insert
     */
    void insertRequest(int requestIdx, int originRequestPredecessorIdx, int destinationRequestPredecessorIdx);

    /**
     * Raw request removal method. Public for debug purposes, but should effectively never be called by an outside member
     * @param requestIdx index of the request we want to remove from the route
     */
    void removeRequest(int requestIdx);

    /**
     * Tries to insert the request origin and destination next to the given origin/destination predecessors. \n \n
     * First we verify multiple constraints that don't require actually inserting the request or doing constraint propagation. \n
     * Then we propagate our min/max bounds, iteratively rippling through every modification induced by min/max neighbour constraints or delta constraints. \n
     * ⚠️ In case of infeasibility, tryAdd automatically reverts changes and the change list will be effectively empty, but otherwise it's the caller's responsibility to revert changes if necessary
     * @param requestIdx Identifier/index in the instance's request vector for the request we wish to insert
     * @param originRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return A change list with every min/max bound change made during the tryAdd procedure and a score estimating insertion quality (lower is better)
     */
    SAEVRouteChangelist tryAddRequest(const int requestIdx, const int originRequestPredecessorIdx, const int destinationRequestPredecessorIdx);

    /**
     * Verifies time window constraints on our request's origin/destination's projected neighbour, aka originPredecessor/destinationPredecessor and their successor.
     * There is a special case taken into account if originPredecessor and destinationPredecessor are the same KeyPoint, since then, Origin's successor would be Destination
     * ⚠️ Weight constraints are checked separately
     * ⚠️ Those checks don't modify the route => no rollback is needed at this point
     * @param requestIdx Index of our request, necessary to retrieve the appropriate key points
     * @param originNodeIndex The request's origin node index, necessary to compute travel times
     * @param destinationNodeIndex The request's destination node index, necessary to compute travel times
     * @param originPredecessor The origin's expected predecessor, aka the point after which we wish to insert our origin
     * @param destinationPredecessor The destination's expected predecessor, aka the point after which we wish to insert our destination
     * @return true iff all neighbouring time window conditions are valid at our insertion points, false otherwise
     */
    bool doNeighbouringTWChecks(const int requestIdx, const int originNodeIndex, const int destinationNodeIndex,
                                const SAEVKeyPoint *originPredecessor, const SAEVKeyPoint *destinationPredecessor);

    /**
     * Method called after having validated conditions not requiring request insertion. \n
     * This will effectively insert our procedure and ripple through bound changes. \n
     * If the bounds become infeasible (min > max), then the propagation stops with a changelist with score= +Infinity and changes will be immediately reverted.
     * Otherwise, it's the responsibility of this method's callers to revert changes if wanted (or to defer this responsibility to its caller)
     * @param requestIdx Identifier/index in the instance's request vector for the request we wish to insert
     * @param originRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return A change list with every min/max bound change made during the insert procedure and a score estimating insertion quality (lower is better)
     */
    SAEVRouteChangelist insertRequestWithPropagation(const int requestIdx, const int originRequestPredecessorIdx, const int destinationRequestPredecessorIdx);

    /**
     * Returns a score value equivalent to the detour implied by insertion of a request after the two given key point indexes.
     * The specific case of origin/destination being inserted one after another is taken into account
     * @param requestIdx Identifier/index in the instance's request vector for the request we wish to insert
     * @param originRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorIdx Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return
     */
    double getDetourScore(const int requestIdx, const int originRequestPredecessorIdx, const int destinationRequestPredecessorIdx);

    SAEVKeyPoint& getOrigin(const size_t requestIdx) { return _route.at(requestIdx*2);}
    SAEVKeyPoint& getDestination(const size_t requestIdx) { return _route.at(requestIdx*2 + 1);}

    SAEVKeyPoint& getOriginDepot(const size_t vehicleId) { return _route.at(_nbRequest + vehicleId*2);}
    SAEVKeyPoint& getDestinationDepot(const size_t vehicleId) { return _route.at(_nbRequest + vehicleId*2 + 1);}
};

#include "propagation/SAEVRouteChangelist.h"

#endif //GREEDYALGORITHM_SAEVROUTE_H
