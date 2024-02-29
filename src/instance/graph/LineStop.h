//
// Created by rbernard on 30/01/24.
//

#ifndef GREEDYALGORITHM_LINESTOP_H
#define GREEDYALGORITHM_LINESTOP_H


#include <utility>

#include "Line.h"

class LineStop {
private:
    Line _lineRef; //reference to the line
    int _stopIndex{}; //index for the stop relative to this node in the Line object

public:
    LineStop() = default;
    LineStop(const Line& lineRef, int stopIndex) : _lineRef(lineRef), _stopIndex(stopIndex) {}

    [[nodiscard]] Line getLineRef() const {
        return _lineRef;
    }

    void setLineRef(Line &lineRef) {
        _lineRef = lineRef;
    }

    /**
     * @return -1 if there are no valid successors to this LineStop's node. Returns the node index in the graph if there is a valid successor
     */
    [[nodiscard]] int getNextNodeIndex() const
    {
        if(_stopIndex + 1 < _lineRef.getNodes().size())
            return _lineRef.getNode(_stopIndex + 1);
        else
            return -1;
    }

    /**
     * @return -1 if there are no valid successors to this LineStop's node. Returns the node index in the graph if there is a valid successor
     */
    [[nodiscard]] int getPrecedingNodeIndex() const
    {
        if(_stopIndex - 1 >= 0)
            return _lineRef.getNode(_stopIndex - 1);
        else
            return -1;
    }

    [[nodiscard]] int getStopIndex() const {
        return _stopIndex;
    }

    void setStopIndex(int stopIndex) {
        _stopIndex = stopIndex;
    }

    [[nodiscard]] bool isEmpty() const{
        return _lineRef.isEmpty();
    }

    bool operator<(LineStop const& rhs) const {return &this->_lineRef < &rhs._lineRef;} //just check line addresses. Basically we just don't want the exact same line twice

    LineStop& operator=(LineStop const& rhs) {
        _lineRef = rhs.getLineRef();
        _stopIndex = rhs.getStopIndex();
        return *this;
    }
};


#endif //GREEDYALGORITHM_LINESTOP_H
