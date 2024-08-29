//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_TRANSITSHORTESTPATH_H
#define GREEDYALGORITHM_TRANSITSHORTESTPATH_H

#include "../ShortestPath.h"
#include "../../instance/graph/LineStop.h"
#include "TransitAlgorithmState.h"

class TransitShortestPath : public ShortestPath<std::reference_wrapper<const LineStop>> {
private:
    int _arrivalTime{-1};
public:
    TransitShortestPath() = default;

    explicit TransitShortestPath(const TransitAlgorithmState& state) : _arrivalTime(state.getInstant()) {
        if(state.getNbConnections() > 0)
            _keyPoints = state.getConnections();
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

    [[nodiscard]] std::reference_wrapper<const LineStop> getDeparture() const override {
        return *_keyPoints.cbegin();
    }

    [[nodiscard]] std::reference_wrapper<const LineStop> getArrival() const override {
        return *(_keyPoints.cend() - 1);
    }

    [[nodiscard]] size_t getDepartureNode() const {
        return getDeparture().get().getNodeIndex();
    }

    [[nodiscard]] size_t getArrivalNode() const {
        return getArrival().get().getNodeIndex();
    }

    [[nodiscard]] int getArrivalTime() const { return _arrivalTime; }
};


#endif //GREEDYALGORITHM_TRANSITSHORTESTPATH_H
