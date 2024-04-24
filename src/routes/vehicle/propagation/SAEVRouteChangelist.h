//
// Created by romain on 23/04/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTECHANGELIST_H
#define GREEDYALGORITHM_SAEVROUTECHANGELIST_H


#include "SAEVRouteChange.h"
#include "../SAEVRoute.h"

/**
 * This class serves as iterative memory for changes made to key points during constraint propagation. \n
 * It memorizes the data to undo/redo request insertion and apply/revert changes made without losing time on duplicate feasibility checks. \n
 * To allow comparison and ordering between multiple insertions if necessary, it also memorizes a score associated with this constraint propagation (lower is better)
 */
class SAEVRouteChangelist {
private:
    SAEVRoute * const _routePtr;
    const int _requestIdx, _originIdx, _destinationIdx;
    std::vector<SAEVRouteChange> _changelist{};
    double _score{std::numeric_limits<double>::infinity()}; //Init score to infinity

public:
    /**
     * Initializes a change list to memorize every iterative modification made during constraint propagation
     * @param routePtr a pointer to the route the constraint propagation was applied on. revert/apply operations will be done on this route
     * @param requestIdx The index of the request in the global request list
     * @param originIdx The index of the request our origin will be inserted after
     * @param destinationIdx The index of the request our destination will be inserted after
     */
    explicit SAEVRouteChangelist(SAEVRoute * const routePtr, const int requestIdx, const int originIdx, const int destinationIdx)
    : _routePtr(routePtr), _requestIdx(requestIdx), _originIdx(originIdx), _destinationIdx(destinationIdx) {};

    /**
     * @return A pointer to the route this change list applies/reverts changes to
     */
    [[nodiscard]] SAEVRoute * getRoutePtr() const;

    [[nodiscard]] const std::vector<SAEVRouteChange> &getChangelist() const;
    /**
     * @return The index of the request that we want to insert to a route
     */
    [[nodiscard]] int getRequestIdx() const;
    /**
     * @return The index of the request our origin will be inserted after
     */
    [[nodiscard]] int getOriginIdx() const;
    /**
     * @return The index of the request our destination will be inserted after
     */
    [[nodiscard]] int getDestinationIdx() const;
    /**
     * @return A score value associated with this changelist. A lower score is better
     */
    [[nodiscard]] double getScore() const;

    /**
     * Updates this change list's score value if needed (namely, after the whole propagation has been done)
     * @param score A value representing how good the insertion associated with this change list is (lower is better)
     */
    void setScore(double score);

    void push_back(SAEVRouteChange change);
    void emplace_back(SAEVRouteChange change);
    void emplace_back(SAEVKeyPoint& kp, Bound bound, int value);

    /**
     * Inserts the request associated to this changelist to the given route and iteratively applies every change memorized in this changelist.
     * Aside from OOB exceptions, no checks are done in this method
     */
    void applyChanges() const;
    /**
     * removes the request associated to this changelist from the given route and iteratively reverts every change memorized in this changelist.
     * Aside from OOB exceptions, no checks are done in this method
     */
    void revertChanges() const;

    /**
     * @param rhs the value *this* will be compared to
     * @return True iff this changelist's score is strictly higher than rhs' score
     */
    bool operator>(const SAEVRouteChangelist& rhs) const;
    /**
     * @param rhs the value *this* will be compared to
     * @return True iff this changelist's score is strictly lower than rhs' score
     */
    bool operator<(const SAEVRouteChangelist& rhs) const;
};


#endif //GREEDYALGORITHM_SAEVROUTECHANGELIST_H
