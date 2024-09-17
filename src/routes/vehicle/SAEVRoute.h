//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"
#include "BestRequestInsertion.h"
#include "BestInsertionQueue.h"

class SAEVRouteChangelist;
class SAEVRoute {
private:
    std::vector<SAEVKeyPoint> _route;
    size_t _nbRequest;
    const Graph* _graph;
    const std::vector<Request>* _requestList;
    size_t _lastActiveVehicleId{0};
public:
    SAEVRoute() = default;
    /**
     * Initializes a route following a tabular doubly chained approach where 0->nbRequest*2-1 are Request Origin (even)/Destination (odd)
     * and nbRequest*2 -> nbRequest*4-1 are vehicle start (even)/end (odd) depot. Each KeyPoint has
     * @param graph the instance's graph, used to get shortest paths values during key point initialization and later in constraint propagation
     * @param requestList the instance's request list, used to associate KeyPoints that represent requests with the appropriate informations
     */
    explicit SAEVRoute(const Graph &graph, const std::vector<Request> &requestList);
    /**
     * Initializes a route following a tabular doubly chained approach where 0->nbRequest*2-1 are Request Origin (even)/Destination (odd)
     * and nbRequest*2 -> nbRequest*4-1 are vehicle start (even)/end (odd) depot. Each KeyPoint has
     * @param graph the instance's graph, used to get shortest paths values during key point initialization and later in constraint propagation
     * @param requestList the instance's request list, used to associate KeyPoints that represent requests with the appropriate informations
     * @param initMultimodal true to init the container to a multimodal approach compatible size in advance, false (default) otherwise to just have requests/depots sections
     */
    explicit SAEVRoute(const Graph &graph, const std::vector<Request> &requestList, bool initMultimodal);

    [[nodiscard]] const std::vector<SAEVKeyPoint> &getRoute() const {
        return _route;
    }

    /**
     * Raw request route insertion method. Public for debug purposes, but should effectively never be called by an outside member
     * @param originKp Reference to the origin key point of the request we want to insert in the route
     * @param originRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede the origin of the request we want to insert
     * @param destinationRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede the destination of the request we want to insert
     */
    void insertRequest(SAEVKeyPoint &originKp, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP);
    /**
     * Inserts a request with propagation in a new vehicle while updating last active vehicle ID (since the vehicle is new, no need to tell where to insert)
     * @param originKp Reference to the origin key point of the request we want to insert in the route
     */
    void insertRequestInNewVehicle(SAEVKeyPoint &originKp);

    /**
     * Raw request removal method. Public for debug purposes, but should effectively never be called by an outside member
     * @param originKp Reference to the origin key point of the request we want to remove from the route
     */
    void removeRequest(SAEVKeyPoint &originKp);

    /**
     * Removes a given base request from the route and does a complete reset and recomputing of bound propagations
     * By nature of our propagation making bounds only smaller and no simple rule existing to make our bounds larger after removal,
     * this method is highly inefficient computationally. A new propagation method or even modelling would be necessary to make it faster
     * @param requestId a base request ID allowing us to find the origin keypoint to use as removal basis
     */
    SAEVRouteChangelist removeRequestWithPropagation(size_t requestId);
    /**
     * Removes a given entry/exit sub-request from the route and does a complete reset and recomputing of bound propagations
     * By nature of our propagation making bounds only smaller and no simple rule existing to make our bounds larger after removal,
     * this method is highly inefficient computationally. A new propagation method or even modelling would be necessary to make it faster
     * @param requestId a base request ID allowing us to find the entry/exit subrequest to delete
     * @param isEntry true iff the subrequest to delete
     */
    SAEVRouteChangelist removeRequestWithPropagation(size_t requestId, bool isEntry);
    /**
     * Removes a given request from the route and does a complete reset and recomputing of bound propagations
     * By nature of our propagation making bounds only smaller and no simple rule existing to make our bounds larger after removal,
     * this method is highly inefficient computationally. A new propagation method or even modelling would be necessary to make it faster
     * @param originKP pointer to the origin key point of a given request to remove
     */
    SAEVRouteChangelist removeRequestWithPropagation(SAEVKeyPoint &originKP);

    /**
     * Updates weight in-between request's Origin/Destination (both included) to account for the given request's weight
     * ⚠️ Execute this function *after* adding the request to the route, as it assumes the request is part of the route
     * @param requestOriginKeyPoint reference to the origin keypoint of the request for which we wish to remove the weight to the route
     */
    void addRequestWeightToRoute(SAEVKeyPoint &requestOriginKeyPoint);
    /**
     * Resets current weight to 0 on the request Origin/Destination key points and decreases weight on the nodes in-between by the request's weight
     * ⚠️ Execute this function *before* removing the request from the route, as it assumes the request is still part of the route
     * @param requestOriginKeyPoint reference to the origin keypoint of the request for which we wish to remove the weight to the route
     */
    void removeRequestWeightFromRoute(SAEVKeyPoint& requestOriginKeyPoint);

    /**
     * Tries to insert the request origin and destination next to the given origin/destination predecessors. \n \n
     * First we verify multiple constraints that don't require actually inserting the request or doing constraint propagation. \n
     * Then we propagate our min/max bounds, iteratively rippling through every modification induced by min/max neighbour constraints or delta constraints. \n
     * ⚠️ In case of infeasibility, tryAdd automatically reverts changes and the change list will be effectively empty, but otherwise it's the caller's responsibility to revert changes if necessary
     * @param requestId Identifier/index in the instance's request vector for the request we wish to insert
     * @param originRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return A change list with every min/max bound change made during the tryAdd procedure and a score estimating insertion quality (lower is better)
     */
    SAEVRouteChangelist tryAddRequest(size_t requestId, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP);

    /**
     * Tries to insert the request origin and destination next to the given origin/destination predecessors. \n \n
     * First we verify multiple constraints that don't require actually inserting the request or doing constraint propagation. \n
     * Then we propagate our min/max bounds, iteratively rippling through every modification induced by min/max neighbour constraints or delta constraints. \n
     * ⚠️ In case of infeasibility, tryAdd automatically reverts changes and the change list will be effectively empty, but otherwise it's the caller's responsibility to revert changes if necessary
     * @param requestOriginKeyPoint KeyPoint reference pointing to the origin KP of the request we wish to insert
     * @param originRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return A change list with every min/max bound change made during the tryAdd procedure and a score estimating insertion quality (lower is better)
     */
    SAEVRouteChangelist tryAddRequest(SAEVKeyPoint &requestOriginKeyPoint, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP);

    /**
     * Verifies time window constraints on our request's origin/destination's projected neighbour, aka originPredecessor/destinationPredecessor and their successor.
     * There is a special case taken into account if originPredecessor and destinationPredecessor are the same KeyPoint, since then, Origin's successor would be Destination
     * ⚠️ Weight constraints are checked separately
     * ⚠️ Those checks don't modify the route => no rollback is needed at this point
     * @param originKP Reference to the origin key point of our request, necessary to retrieve the appropriate key points
     * @param originNodeIndex The request's origin node index, necessary to compute travel times
     * @param destinationNodeIndex The request's destination node index, necessary to compute travel times
     * @param originPredecessor The origin's expected predecessor, aka the point after which we wish to insert our origin
     * @param destinationPredecessor The destination's expected predecessor, aka the point after which we wish to insert our destination
     * @return true iff all neighbouring time window conditions are valid at our insertion points, false otherwise
     */
    bool doNeighbouringTWChecks(const SAEVKeyPoint &originKP, const SAEVKeyPoint *originPredecessor,
                                const SAEVKeyPoint *destinationPredecessor) const;

    /**
     * Method called after having validated conditions not requiring request insertion. \n
     * This will effectively insert our procedure and ripple through bound changes. \n
     * If the bounds become infeasible (min > max), then the propagation stops with a changelist with score= +Infinity and changes will be immediately reverted.
     * Otherwise, it's the responsibility of this method's callers to revert changes if wanted (or to defer this responsibility to its caller)
     * @param originKP Reference to the origin key point for the request we wish to insert
     * @param originRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return A change list with every min/max bound change made during the insert procedure and a score estimating insertion quality (lower is better)
     */
    SAEVRouteChangelist insertRequestWithPropagation(SAEVKeyPoint &originKP, SAEVKeyPoint * originRequestPredecessorKP, SAEVKeyPoint * destinationRequestPredecessorKP);

    /**
     * Executes the bound propagation algorithm, starting from the given bound propagation queue.
     * Each item in the queue references which bound (Min/Max) and what key point was affected, and from there we verify bounds for each predecessor/successor
     *
     * If the bounds become infeasible (min > max), then the propagation stops with a changelist with score= +Infinity and changes will be immediately reverted.
     * Otherwise, it's the responsibility of this method's callers to revert changes if wanted (or to defer this responsibility to its caller)
     *
     * @param boundPropagationQueue a (Bound/Keypoint) pair queue giving a list of starting points for propagation. Typically when inserting a key point, the queue contains two items (one for each bound)
     * @return A change list with every min/max bound change made during the insert procedure and a score estimating insertion quality (lower is better)
     */ // FIXME: not a fan of using int instead of the Bound enum here, but I can't forward-declare it since it's part of SAEVRouteChange
    SAEVRouteChangelist doBoundPropagation(std::queue<std::pair<int, SAEVKeyPoint *>> &boundPropagationQueue,
                                           SAEVRouteChangelist &changelist);
    /**
     * Returns a score value equivalent to the detour implied by insertion of a request after the two given key point indexes.
     * The specific case of origin/destination being inserted one after another is taken into account
     * ⚠️ This method assumes the insertion hasn't been done yet, so compute detour scores before inserting your request ⚠️
     * @param originKP Reference to the origin key point for the request we wish to insert
     * @param originRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's origin in the route
     * @param destinationRequestPredecessorKP Identifier/index in the route key point vector for the request origin or destination that will precede our request's destination in the route
     * @return
     */
    double getDetourScore(const SAEVKeyPoint &originKP, const SAEVKeyPoint * originRequestPredecessorKP, const SAEVKeyPoint * destinationRequestPredecessorKP) const;

    BestInsertionQueue getBestInsertionsQueue(size_t requestId, size_t vehicleId);
    BestInsertionQueue getBestInsertionsQueue(SAEVKeyPoint &originKP, size_t vehicleId);
    BestInsertionQueue getBestFeasibleInsertionsQueue(size_t requestId, size_t vehicleId);
    BestInsertionQueue getBestFeasibleInsertionsQueue(SAEVKeyPoint &originKP, size_t vehicleId);
    void getBestFeasibleInsertionsQueue(BestInsertionQueue& bestInsertionQueue, const SAEVKeyPoint &requestOriginKeyPoint, size_t vehicleId);

    SAEVKeyPoint& getRequestOrigin(const size_t requestId) { return _route[getRequestOriginRouteIdx(requestId)];}
    SAEVKeyPoint& getRequestDestination(const size_t requestId) { return _route[getRequestDestinationRouteIdx(requestId)];}

    SAEVKeyPoint& getOriginDepot(const size_t vehicleId) { return _route[getOriginDepotRouteIdx(vehicleId)];}
    SAEVKeyPoint& getDestinationDepot(const size_t vehicleId) { return _route[getDestinationDepotRouteIdx(vehicleId)];}

    [[nodiscard]] size_t getRequestOriginRouteIdx(const size_t requestId) const { return requestId * 2;}
    [[nodiscard]] size_t getRequestDestinationRouteIdx(const size_t requestId) const { return requestId * 2 + 1;}
    [[nodiscard]] size_t getRequestOriginNodeIdx(const size_t requestId) const { return _route[getRequestOriginRouteIdx(requestId)].getNodeIndex();}
    [[nodiscard]] size_t getRequestDestinationNodeIdx(const size_t requestId) const { return _route[getRequestDestinationRouteIdx(requestId)].getNodeIndex();}

    [[nodiscard]] size_t getOriginDepotRouteIdx(const size_t vehicleId) const { return _nbRequest*2 + vehicleId*2;}
    [[nodiscard]] size_t getDestinationDepotRouteIdx(const size_t vehicleId) const { return _nbRequest*2 + vehicleId*2 + 1;}
    [[nodiscard]] size_t getOriginDepotNodeIdx(const size_t vehicleId) const { return _route[getOriginDepotRouteIdx(vehicleId)].getNodeIndex();}
    [[nodiscard]] size_t getDestinationDepotNodeIdx(const size_t vehicleId) const { return _route[getDestinationDepotRouteIdx(vehicleId)].getNodeIndex();}

    [[nodiscard]] size_t getEntrySubRequestOriginRouteIdx(const size_t requestId) const { return _nbRequest * 4 + requestId * 2;}
    [[nodiscard]] size_t getEntrySubRequestDestinationRouteIdx(const size_t requestId) const { return _nbRequest * 4 + requestId * 2 + 1;}
    [[nodiscard]] SAEVKeyPoint& getEntrySubRequestOrigin(const size_t requestId) { return _route[getEntrySubRequestOriginRouteIdx(requestId)];}
    [[nodiscard]] SAEVKeyPoint& getEntrySubRequestDestination(const size_t requestId) { return _route[getEntrySubRequestDestinationRouteIdx(requestId)];}
    [[nodiscard]] size_t getEntrySubRequestOriginNodeIdx(const size_t requestId) const { return _route[getEntrySubRequestOriginRouteIdx(requestId)].getNodeIndex();}
    [[nodiscard]] size_t getEntrySubRequestDestinationNodeIdx(const size_t requestId) const { return _route[getEntrySubRequestDestinationRouteIdx(requestId)].getNodeIndex();}

    [[nodiscard]] size_t getExitSubRequestOriginRouteIdx(const size_t requestId) const { return _nbRequest * 6 + requestId * 2;}
    [[nodiscard]] size_t getExitSubRequestDestinationRouteIdx(const size_t requestId) const { return _nbRequest * 6 + requestId * 2 + 1;}
    [[nodiscard]] SAEVKeyPoint& getExitSubRequestOrigin(const size_t requestId) { return _route[getExitSubRequestOriginRouteIdx(requestId)];}
    [[nodiscard]] SAEVKeyPoint& getExitSubRequestDestination(const size_t requestId) { return _route[getExitSubRequestDestinationRouteIdx(requestId)];}
    [[nodiscard]] size_t getExitSubRequestOriginNodeIdx(const size_t requestId) const { return _route[getExitSubRequestOriginRouteIdx(requestId)].getNodeIndex();}
    [[nodiscard]] size_t getExitSubRequestDestinationNodeIdx(const size_t requestId) const { return _route[getExitSubRequestDestinationRouteIdx(requestId)].getNodeIndex();}

    [[nodiscard]] size_t getLastActiveVehicleId() const { return _lastActiveVehicleId; }
    size_t addNewActiveVehicle() {
        assertm(_lastActiveVehicleId < _nbRequest - 1, "No more vehicle available");
        return ++_lastActiveVehicleId;
    }
    void setLastActiveVehicleId(size_t lastActiveVehicleId) { _lastActiveVehicleId = lastActiveVehicleId; }

    /**
     * Verifies that time windows have been properly propagated for a given vehicle's route
     * @param vehicleId
     * @return true iff all time windows have been properly shrunk
     */
    bool checkRouteTimeWindows(size_t vehicleId);

    /**
     * @return A string comprised of every vehicle appended to their own line each
     */
    std::string to_string();
    /**
     * @param vehicleId ID of the vehicle for which we want to print the current route
     * @return A string comprised of every keypoint traversed by the vehicle corresponding to the given vehicleId
     */
    std::string to_string(size_t vehicleId);
    void exportToFile();
};

#include "propagation/SAEVRouteChangelist.h"

#endif //GREEDYALGORITHM_SAEVROUTE_H
