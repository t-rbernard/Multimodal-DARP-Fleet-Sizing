//
// Created by romain on 22/01/24.
//

#include "Edge.h"

Node *Edge::getIn() const {
    return in;
}

void Edge::setIn(Node *pNode) {
    Edge::in = pNode;
}

Node *Edge::getOut() const {
    return out;
}

void Edge::setOut(Node *pNode) {
    Edge::out = pNode;
}

double Edge::getLength() const {
    return length;
}

void Edge::setLength(double d) {
    Edge::length = d;
}
