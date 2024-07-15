//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_SHORTESTPATH_H
#define GREEDYALGORITHM_SHORTESTPATH_H

#include <vector>

template <typename KeyPoint>
class ShortestPath {
protected:
    std::vector<KeyPoint> _keyPoints;


public:
    explicit ShortestPath() = default;

    [[nodiscard]] const std::vector<KeyPoint> &getKeyPoints() const {
        return _keyPoints;
    }

    virtual const KeyPoint* getDeparture() const {
        return _keyPoints.cbegin().base();
    }

    virtual const KeyPoint* getArrival() const {
        return (_keyPoints.cend() - 1).base();
    }

    void replaceKeyPoint(int keyPointIndex, KeyPoint& value) {
        _keyPoints[keyPointIndex] = value;
    }

    /**
     * Adds reference to a key point to the shortest path (e.g : a node index, a Line Stop)
     * @param keyPoint The key point to add at the end of the key points vector
     */
    void addKeyPoint(KeyPoint& keyPoint) {
        _keyPoints.emplace_back(keyPoint);
    }
};


#endif //GREEDYALGORITHM_SHORTESTPATH_H
