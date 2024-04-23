//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_SAEVROUTE_H
#define GREEDYALGORITHM_SAEVROUTE_H


#include <vector>
#include "SAEVKeyPoint.h"

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
     * Raw request route insertion method. Public for debug purposes, but should effectively never be called by an outside member FIXME:make private
     * @param requestIdx index of the request we want to insert in the route
     * @param originRequestPredecessorIdx Index of the request that will precede the origin of the request we want to insert
     * @param destinationRequestPredecessorIdx Index of the request that will precede the destination of the request we want to insert
     */
    void insertRequest(int requestIdx, int originRequestPredecessorIdx, int destinationRequestPredecessorIdx);

    void insertRequestWithPropagation(int requestIdx, int originRequestPredecessorIdx, int destinationRequestPredecessorIdx);

    /**
     * Raw request removal method. Public for debug purposes, but should effectively never be called by an outside member FIXME:make private
     * @param requestIdx index of the request we want to remove from the route
     */
    void removeRequest(int requestIdx);

    void removeRequestWithPropagation(int requestIdx);
};


#endif //GREEDYALGORITHM_SAEVROUTE_H
