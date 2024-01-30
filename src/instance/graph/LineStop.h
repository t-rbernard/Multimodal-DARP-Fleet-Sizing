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
    LineStop(Line lineRef, int stopIndex) : _lineRef(std::move(lineRef)), _stopIndex(stopIndex) {}

    [[nodiscard]] Line getLineRef() const {
        return _lineRef;
    }

    void setLineRef(Line &lineRef) {
        _lineRef = lineRef;
    }

    [[nodiscard]] int getStopIndex() const {
        return _stopIndex;
    }

    void setStopIndex(int stopIndex) {
        _stopIndex = stopIndex;
    }

    bool operator<(LineStop const& rhs) const {return &this->_lineRef < &rhs._lineRef;} //just check line addresses. Basically we just don't want the exact same line twice
};


#endif //GREEDYALGORITHM_LINESTOP_H
