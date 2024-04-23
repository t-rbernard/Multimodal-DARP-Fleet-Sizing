//
// Created by romain on 22/03/24.
//

#include "KeyPoint.h"

KeyPoint::KeyPoint() = default;
KeyPoint::KeyPoint(int nodeIndex) : _nodeIndex(nodeIndex) {}

int KeyPoint::getNodeIndex() const {
    return _nodeIndex;
}

void KeyPoint::setNodeIndex(int nodeIndex) {
    KeyPoint::_nodeIndex = nodeIndex;
}