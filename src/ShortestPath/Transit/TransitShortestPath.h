//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATH_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATH_H

#include "../ShortestPath.h"
#include "../../instance/graph/LineStop.h"
#include "TransitAlgorithmState.h"

class TransitShortestPath : public ShortestPath<LineStop> {
public:

    /**
     * Strict dominance between two transit shortest path
     * @param rhs
     * @return
     */
    [[nodiscard]] bool strictlyDominates(const TransitShortestPath& rhs) const {
        return this->getKeyPoints().size() <= rhs.getKeyPoints().size()
               && this->getDuration() <= rhs.getDuration();
    }

    bool operator<(const TransitShortestPath& rhs) const {
        return this->getDuration() < rhs.getDuration() ||
               (this->getDuration() == rhs.getDuration() && this->getKeyPoints().size() < rhs.getKeyPoints().size());
    }

    bool operator>(const TransitShortestPath& rhs) const {
        return this->getDuration() > rhs.getDuration() ||
               (this->getDuration() == rhs.getDuration() && this->getKeyPoints().size() > rhs.getKeyPoints().size());
    }


//    FIXME: check if I can properly remove this or if shortest path comparisons may be useful
//    [[nodiscard]] bool strictlyDominates(const TransitAlgorithmState& rhs) const {
//        return this->getKeyPoints().size() <= rhs.getConnections().size()
//               && this->getDuration() <= rhs.getInstant();
//    }
//
//    bool operator<(const TransitAlgorithmState& rhs) const {
//        return this->getDuration() < rhs.getInstant() ||
//               (this->getDuration() == rhs.getInstant() && this->getKeyPoints().size() < rhs.getConnections().size());
//    }
//
//    bool operator>(const TransitAlgorithmState& rhs) const {
//        return this->getDuration() > rhs.getInstant() ||
//               (this->getDuration() == rhs.getInstant() && this->getKeyPoints().size() > rhs.getConnections().size());
//    }
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATH_H
