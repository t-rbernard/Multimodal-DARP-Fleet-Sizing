//
// Created by romain on 22/01/24.
//

#include "Edge.h"

int Edge::getNodeStart() const {
    return _start;
}

void Edge::setNodeStart(int start) {
    Edge::_start = start;
}

int Edge::getNodeEnd() const {
    return _end;
}

void Edge::setNodeEnd(int end) {
    Edge::_end = end;
}

double Edge::getLength() const {
    return _length;
}

void Edge::setLength(double d) {
    Edge::_length = d;
}

Edge::Edge(int start, int end, double length) {
    _start = start;
    _end = end;
    _length = length;
}
