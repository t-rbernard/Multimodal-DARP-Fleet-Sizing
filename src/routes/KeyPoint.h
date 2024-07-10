//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_KEYPOINT_H
#define GREEDYALGORITHM_KEYPOINT_H


#include <string>
#include "../TimeWindow.h"

class KeyPoint {
private:
    int _nodeIndex{-1};

public:
    KeyPoint();
    explicit KeyPoint(int nodeIndex);

    [[nodiscard]] int getNodeIndex() const;
    void setNodeIndex(int nodeIndex);

    [[nodiscard]] virtual bool check() const { return true; };

    virtual ~KeyPoint() = default;

    virtual std::string to_string() const { return std::to_string(_nodeIndex); }
};


#endif //GREEDYALGORITHM_KEYPOINT_H
