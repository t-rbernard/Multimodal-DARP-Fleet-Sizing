//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_NODE_H
#define GREEDYALGORITHM_NODE_H

#include <string>
#include <unordered_set>
#include <set>
#include <unordered_map>

enum Status {
    work,
    leisure,
    residential
};

static std::unordered_map<std::string,Status> const stringToStatusMap = {{"work",        Status::work}, {"leisure", Status::leisure},
                                                                         {"residential", Status::residential} };

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

    struct Coordinate {
        double x,y;
    };

    bool isPTNode();
    void addBusLine(const Line& line, int indexInLine);
    Status const statusFromString(std::string from);
    [[nodiscard]] Coordinate getCoordinates() const {return Coordinate(_x,_y);}
    [[nodiscard]] std::set<LineStop> getPTLinesSet() const {return _ptLines;}

    /**
     * Verify if _x, _y and _status are equal to check for node equality
     * @param rhs right hand side of the comparison operator
     * @return True iff _x, _y and _status are equal between both sides of the operator
     */
    bool operator==(const Node& rhs) const;
    /**
     * Verify if _x, _y or _status are different to check for node difference
     * @param rhs right hand side of the comparison operator
     * @return True if any of _x, _y and _status is not equal between both sides of the operator, false otherwise
     */
    bool operator!=(const Node& rhs) const;
};
#include "Line.h"
#include "LineStop.h"


#endif //GREEDYALGORITHM_NODE_H
