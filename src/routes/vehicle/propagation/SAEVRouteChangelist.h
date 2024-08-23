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
public:
    enum class InsertionStatus{
        SUCCESS,
        FAILURE_MIN,
        FAILURE_MAX,
        FAILURE_DELTA_MIN,
        FAILURE_DELTA_MAX,
        FAILURE_PRECONDITION_TW, //To check if you need to remove a request, check status > FAILURE_PRECONDITION_WEIGHT
        FAILURE_PRECONDITION_WEIGHT,
        CHANGELIST_REVERTED
    };
private:
    //Change target info
    SAEVRoute * _routePtr{};
    SAEVKeyPoint* _requestOriginKeyPoint{};
    size_t _vehicleId{};
    SAEVKeyPoint* _originPredecessorKP{};
    SAEVKeyPoint* _destinationPredecessorKP{};

    //Changelist specific info (aka, if this ever becomes a template class, *this* is what will go in the generic interface)
    std::vector<SAEVRouteChange> _changelist{};
    double _score{std::numeric_limits<double>::infinity()}; //Init score to infinity

    InsertionStatus _status{InsertionStatus::CHANGELIST_REVERTED}; //Status on changelist creation
    InsertionStatus _currentStatus; //Updated status after applying/reverting changelist

public:

    SAEVRouteChangelist(SAEVRoute * routePtr, SAEVKeyPoint *requestOriginKeyPoint) : _routePtr(routePtr), _requestOriginKeyPoint(requestOriginKeyPoint) {}

    /**
     * Initializes a change list to memorize every iterative modification made during constraint propagation
     * @param routePtr a pointer to the route the constraint propagation was applied on. revert/apply operations will be done on this route
     * @param requestOriginKeyPoint Reference to the request origin key point in the route
     * @param originPredecessorKP Reference to the request KP our origin will be inserted after
     * @param destinationPredecessorKP Reference to the request KP our destination will be inserted after
     */
    explicit SAEVRouteChangelist(SAEVRoute * const routePtr, SAEVKeyPoint *requestOriginKeyPoint, SAEVKeyPoint* originPredecessorKP, SAEVKeyPoint* destinationPredecessorKP, InsertionStatus status)
    : _routePtr(routePtr), _requestOriginKeyPoint(requestOriginKeyPoint), _originPredecessorKP(originPredecessorKP), _destinationPredecessorKP(destinationPredecessorKP), _status(status), _currentStatus(status) {};


    /**
     * @return A pointer to the route this change list applies/reverts changes to
     */
    [[nodiscard]] SAEVRoute * getRoutePtr() const;

    [[nodiscard]] const std::vector<SAEVRouteChange> &getChangelist() const;

    SAEVKeyPoint *getRequestOriginKeyPoint() const;

    [[nodiscard]] size_t getVehicleId() const;
    void setVehicleId(size_t vehicleId);

    [[nodiscard]] const SAEVKeyPoint * getOriginPredecessorKP() const;

    [[nodiscard]] const SAEVKeyPoint * getDestinationPredecessorKP() const;

    [[nodiscard]] InsertionStatus getStatus() const;
    void setStatus(InsertionStatus status);
    [[nodiscard]] bool success();

    [[nodiscard]] InsertionStatus getCurrentStatus() const;
    void setCurrentStatus(InsertionStatus currentStatus);

    /**
     * @return A score value associated with this changelist. A lower score is better
     */
    [[nodiscard]] double getScore() const;

    /**
     * @return True iff the current state of the route wrt change list requires to remove undo an insertion
     */
    [[nodiscard]] bool shouldUndoInsertion() const;

    /**
     * Updates this change list's score value if needed (namely, after the whole propagation has been done)
     * @param score A value representing how good the insertion associated with this change list is (lower is better)
     */
    void setScore(double score);

    void push_back(SAEVRouteChange change);
    void emplace_back(SAEVRouteChange change);
    void emplace_back(SAEVKeyPoint& kp, Bound bound, uint value);

    /**
     * Inserts the request associated to this changelist to the given route and iteratively applies every change memorized in this changelist.
     * Changelist status is updated to reflect the original value associated with the changes
     * Aside from OOB exceptions, no checks are done in this method
     */
    void applyChanges();
    /**
     * removes the request associated to this changelist from the given route and iteratively reverts every change memorized in this changelist.
     * Aside from OOB exceptions, no checks are done in this method
     */
    void revertChanges();

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
