//
// Created by rbernard on 22/01/24.
//

#include "Edge.h"

size_t Edge::getStartNodeIdx() const {
    return _startNodeIdx;
}

void Edge::setStartNodeIdx(size_t startNodeIdx) {
    Edge::_startNodeIdx = startNodeIdx;
}

size_t Edge::getEndNodeIdx() const {
    return _endNodeIdx;
}

void Edge::setEndNodeIdx(size_t endNodeIdx) {
    Edge::_endNodeIdx = endNodeIdx;
}

uint Edge::getLength() const {
    return _length;
}

void Edge::setLength(uint length) {
    Edge::_length = length;
}
