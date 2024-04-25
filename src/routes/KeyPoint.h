//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_KEYPOINT_H
#define GREEDYALGORITHM_KEYPOINT_H


#include "../TimeWindow.h"

class KeyPoint {
protected:
    int _nodeIndex{-1};

public:
    KeyPoint();
    explicit KeyPoint(int nodeIndex);

    [[nodiscard]] int getNodeIndex() const;
    void setNodeIndex(int nodeIndex);

    [[nodiscard]] virtual bool check() const { return true; };

    ~KeyPoint() = default;
};


#endif //GREEDYALGORITHM_KEYPOINT_H
