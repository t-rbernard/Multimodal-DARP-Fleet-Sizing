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

Node::Node(Status status, double x, double y) : _status(status), _x(x), _y(y), _ptLines(std::vector<LineStop>()) {
    this->_status = status;
    this->_x = x;
    this->_y = y;
}

void Node::addBusLine(const Line& line, int indexInLine) {
    this->_ptLines.emplace_back(line, indexInLine);
}

bool Node::operator==(const Node &rhs) const {
    return _x == rhs.getX()
        && _y == rhs.getY()
        && _status == rhs.getStatus();
}

bool Node::operator!=(const Node &rhs) const {
    return _x != rhs.getX()
           || _y != rhs.getY()
           || _status != rhs.getStatus();
}

Status  Node::statusFromString(std::string from) {
    auto pair = stringToStatusMap.find(from);
    if (pair != stringToStatusMap.end()) {
        return pair->second;
    } else {
        return Status::work;
    }
}

void Node::emplaceBackClosestStation(size_t closestStationIdx) {
    _closestStationsNodeIdxVector.emplace_back(closestStationIdx);
}

const std::vector<size_t> Node::getClosestStationsNodeIdxVector() {
    return _closestStationsNodeIdxVector;
}
