//
// Created by rbernard on 22/01/24.
//

#ifndef GREEDYALGORITHM_NODE_H
#define GREEDYALGORITHM_NODE_H

#include <string>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include "../../ShortestPath/Vehicle/ClosestDestinationsContainer.h"

/**
 * Status used to qualify nodes as places with different goals (work, leisure, residential)
 * Useful for a semi-random requests generation
 */
enum Status {
    work,
    leisure,
    residential
};

static std::unordered_map<std::string,Status> const stringToStatusMap = {{"work",        Status::work},
                                                                         {"leisure",     Status::leisure},
                                                                         {"residential", Status::residential} };

class Line;
class LineStop;
class Node {
private:
    Status _status;
    double _x;
    double _y;
    std::vector<LineStop> _ptLines;
    std::vector<int> _incomingEdgesIndex; //List of edge index in the graph structure for all edges leading to this node
    std::vector<int> _outgoingEdgesIndex; //List of edge index in the graph structure for all edges leading to this node
    std::vector<size_t> _bestStationsNodeIdxVector;
    //TODO : Should these vectors be considered complete over the whole set of nodes ? Probably ? Considering we will probably pre-process shortest paths between all SAEV stations

public:
    Node();
    Node(Status status, double x, double y);

    /**
     * Simple struct to return full coordinates data with an x and y pair
     */
    struct Coordinate {
        double x,y;
    };

    /**
     * @return True if this node contains one or more LineStop
     */
    bool isPTNode() const;
    /**
     * Adds a new LineStop to the LineStop set
     * @param line the line referenced by the LineStop
     * @param indexInLine The station index in the Line for the current node
     */
    void addBusLine(const Line& line, int indexInLine);
    /**
     * Parses the string parameter and converts it to an appropriate Status value
     * @param from String serving as basis for conversion (trailing spaces and capitalization don't matter)
     * @return A status depending on the given string. If no status corresponds to the given String, will default to Work
     */
    static Status statusFromString(const std::string& from) ;
    /**
     * Formats x and y data in a Coordinate object and returns it
     * @return A new Coordinate object with x and y data
     */
    [[nodiscard]] Coordinate getCoordinates() const {return Coordinate(_x,_y);}
    [[nodiscard]] double getX() const {return _x;}
    [[nodiscard]] double getY() const {return _y;}
    [[nodiscard]] Status getStatus() const {return _status;}
    [[nodiscard]] const std::vector<int> & getIncomingEdges() const {return _incomingEdgesIndex;}
    [[nodiscard]] const std::vector<int> & getOutgoingEdges() const {return _outgoingEdgesIndex;}
    [[nodiscard]] const std::vector<LineStop> & getPTLinesSet() const {return _ptLines;}

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

    void emplaceBackClosestStation(size_t closestStationIdx);
    void emplaceBackOutgoingEdge(size_t outgoingEdgeIndex);
    void emplaceBackIncomingEdge(size_t incomingEdgeIndex);

    [[nodiscard]] const std::vector<size_t>& getBestStationsNodeIdxVector() const;
    void setBestStationsNodeIdxVector(const std::vector<size_t> &bestStationsOrderedVector);

    void clearPTLineSet();
};
#include "Line.h"
#include "LineStop.h"


#endif //GREEDYALGORITHM_NODE_H
