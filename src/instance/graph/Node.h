//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_NODE_H
#define GREEDYALGORITHM_NODE_H

#include <string>
#include <unordered_set>
#include <set>

enum Status {
    work,
    leisure,
    residential
};

class Line;
class LineStop;
class Node {
    private:
        Status _status;
        double _x;
        double _y;
        std::set<LineStop> _ptLines;

    public:
        Node();
        Node(Status status, double x, double y);
        bool isPTNode();
        void addBusLine(const Line& line, int nodeInLine);
        Status statusFromString(std::string from);
};
#include "Line.h"
#include "LineStop.h"


#endif //GREEDYALGORITHM_NODE_H
