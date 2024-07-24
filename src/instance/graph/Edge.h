//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_EDGE_H
#define GREEDYALGORITHM_EDGE_H


#include "Node.h"

class Edge {
private:
    size_t _startNodeIdx;
    size_t _endNodeIdx;
    uint _length;

public:
    [[nodiscard]] size_t getStartNodeIdx() const;
    void setStartNodeIdx(size_t startNodeIdx);

    [[nodiscard]] size_t getEndNodeIdx() const;
    void setEndNodeIdx(size_t endNodeIdx);

    [[nodiscard]] uint getLength() const;
    void setLength(uint length);

    Edge(size_t startNodeIdx, size_t endNodeIdx, uint length) : _startNodeIdx(startNodeIdx), _endNodeIdx(endNodeIdx), _length(length) {}
};


#endif //GREEDYALGORITHM_EDGE_H
