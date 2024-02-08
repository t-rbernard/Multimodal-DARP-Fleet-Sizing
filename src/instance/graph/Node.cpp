//
// Created by rbernard on 22/01/24.
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
    return &_ptLines == nullptr || _ptLines.empty();
}

Node::Node(Status status, double x, double y) : _status(status), _x(x), _y(y), _ptLines(std::set<LineStop>()) {
    this->_status = status;
    this->_x = x;
    this->_y = y;
}

void Node::addBusLine(const Line& line, int indexInLine) {
    this->_ptLines.emplace(line, indexInLine);
}
