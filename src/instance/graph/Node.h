//
// Created by romain on 22/01/24.
//

#ifndef GREEDYALGORITHM_NODE_H
#define GREEDYALGORITHM_NODE_H

#include <vector>
#include "Line.h"

enum Status {
    work,
    leisure,
    residential
};

class Node {
    private:
        Status _status;
        double _x;
        double _y;
        std::vector<std::pair<Line, Node>> _busLines;

    public:
        Node();
        Node(Status status, double x, double y);
        bool isPTNode();
        bool addBusLine(Line line, Node node); //TODO: probably replace with node reference/int or smth
};


#endif //GREEDYALGORITHM_NODE_H
