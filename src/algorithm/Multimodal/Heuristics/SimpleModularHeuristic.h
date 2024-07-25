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

class SimpleModularHeuristic {
    const Graph* _graph{nullptr};
    SAEVRoute* _route{nullptr};

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
    std::vector<TransitAccess> getBestTransitEntriesList(const Request &baseRequest);

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

    //Entry filter
    uint getMinEntryConstraint(Request request, size_t node);
    uint getMaxEntryConstraint(Request request, size_t node);
    //Exit filter
    uint getMinExitConstraint(Request request, size_t node);
    uint getMaxExitConstraint(Request request, size_t node);
};


#endif //GREEDYALGORITHM_SIMPLEMODULARHEURISTIC_H
