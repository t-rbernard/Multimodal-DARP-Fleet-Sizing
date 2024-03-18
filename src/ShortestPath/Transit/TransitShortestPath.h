//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATH_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATH_H

#include "../ShortestPath.h"
#include "../../instance/graph/LineStop.h"
#include "TransitAlgorithmState.h"

class TransitShortestPath : public ShortestPath<LineStop> {
private:
    int _arrivalTime;
public:

    explicit TransitShortestPath(const TransitAlgorithmState& state) {
        _arrivalTime = state.getInstant();
        std::move(state.getConnections().begin(), state.getConnections().end(),_keyPoints.begin());
    }

    /**
     * Strict dominance between two transit shortest path
     * @param rhs
     * @return
     */
    [[nodiscard]] bool strictlyDominates(const TransitShortestPath& rhs) const {
        return this->getKeyPoints().size() <= rhs.getKeyPoints().size()
               && this->getArrivalTime() <= rhs.getArrivalTime();
    }

    bool operator<(const TransitShortestPath& rhs) const {
        return this->getArrivalTime() < rhs.getArrivalTime() ||
               (this->getArrivalTime() == rhs.getArrivalTime() && this->getKeyPoints().size() < rhs.getKeyPoints().size());
    }

    bool operator>(const TransitShortestPath& rhs) const {
        return this->getArrivalTime() > rhs.getArrivalTime() ||
               (this->getArrivalTime() == rhs.getArrivalTime() && this->getKeyPoints().size() > rhs.getKeyPoints().size());
    }

    [[nodiscard]] int getArrivalTime() const { return _arrivalTime; }


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
