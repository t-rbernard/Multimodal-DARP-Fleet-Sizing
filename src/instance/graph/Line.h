//
// Created by Bebs on 29/01/2024.
//

#ifndef GREEDYALGORITHM_LINE_H
#define GREEDYALGORITHM_LINE_H


#include <vector>
#include "Node.h"

class Line {
private:
    std::vector<Node> nodes; //sorted according to line order (start -> terminus)
    std::vector<std::vector<int>> timetable; //list of list of timestamps for each node in order (size of timetable must remain constant throughout the whole vector

public:
    Line() = default;
};


#endif //GREEDYALGORITHM_LINE_H
