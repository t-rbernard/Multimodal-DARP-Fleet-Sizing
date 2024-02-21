//
// Created by Bebs on 20/02/2024.
//

#ifndef GREEDYALGORITHM_SHORTESTPATH_H
#define GREEDYALGORITHM_SHORTESTPATH_H

#include <vector>

template <typename KeyPoint>
class ShortestPath {
    int _duration;
    std::vector<KeyPoint> _keyPoints;

    explicit ShortestPath(int duration) { _duration = duration;}

public:
    [[nodiscard]] int getDuration() const {
        return _duration;
    }

    void setDuration(int duration) {
        _duration = duration;
    }

    [[nodiscard]] const std::vector<KeyPoint> &getKeyPoints() const {
        return _keyPoints;
    }

    void replaceKeyPoint(int keyPointIndex, KeyPoint& value) {
        _keyPoints.assign(keyPointIndex, value);
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
