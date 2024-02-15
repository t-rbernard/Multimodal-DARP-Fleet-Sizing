//
// Created by rbernard on 29/01/2024.
//

#ifndef GREEDYALGORITHM_LINE_H
#define GREEDYALGORITHM_LINE_H


#include <utility>
#include <vector>

class Line {
private:
    std::vector<int> _nodes; //index according to Graph::_nodes, sorted according to line order (start -> terminus)
    std::vector<std::vector<int>> _timetables; //list of list of timestamps for each node start order (size of _timetables must remain constant throughout the whole vector
    bool checkSchedules();
public:
    Line() = default;

    std::vector<int> getNodes() { return _nodes;};
    int getNode(int index) { return _nodes.at(index);};
    void addNode(const int node){this->_nodes.emplace_back(node);};

    std::vector<int> getTimetable(int pos) { return _timetables.at(pos);};
    std::vector<std::vector<int>> getTimetables() { return _timetables;};
    void addTimetable(const std::vector<int>& timetable) { _timetables.push_back(timetable);};
    void setTimetable(int pos, const std::vector<int>& timetable) { _timetables.insert(_timetables.begin() + pos, timetable);};
    //TODO : for unit tests, add Line from file creation function
    //  check(), checkSchedules(), checkNodesLink(nodesvector)
    //  Additionally, Line verifier utility functions could be good here (both debug, unit tests and instance verification => Graph.check() checks all subcomponent)
    bool check();
};

#include "Node.h"

#endif //GREEDYALGORITHM_LINE_H
