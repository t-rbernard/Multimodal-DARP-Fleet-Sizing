//
// Created by rbernard on 29/01/2024.
//

#ifndef GREEDYALGORITHM_LINE_H
#define GREEDYALGORITHM_LINE_H


#include <utility>
#include <vector>
#include "Node.h"

class Line {
private:
    std::vector<Node> _nodes; //sorted according to line order (start -> terminus)
    std::vector<std::vector<int>> _timetable; //list of list of timestamps for each node start order (size of _timetable must remain constant throughout the whole vector

public:
    Line() = default;
    void addNode(Node node) { _nodes.push_back(node); };
    std::vector<int> getTimetable(int pos) { return _timetable.at(pos);};
    void setTimetable(int pos, std::vector<int> timetable) { _timetable.at(pos) = std::move(timetable);};
};


#endif //GREEDYALGORITHM_LINE_H
