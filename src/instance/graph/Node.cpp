//
// Created by romain on 22/01/24.
//

#include "Node.h"

/**
 * Default constructor, coordinates default to negatives and _status to work
 */
Node::Node() {
    _status = Status::work;
    _x = -1;
    _y = -1;
}

bool Node::isPTNode() {
    return &_busLines == nullptr || _busLines.empty();
}

Node::Node(Status status, double x, double y) {
    this->_status = status;
    this->_x = x;
    this->_y = y;
}

bool Node::addBusLine(Line line, Node node) {
    return false;
}
