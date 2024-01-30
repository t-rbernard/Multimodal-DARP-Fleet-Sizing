//
// Created by romain on 22/01/24.
//

#ifndef GREEDYALGORITHM_EDGE_H
#define GREEDYALGORITHM_EDGE_H


#include "Node.h"

class Edge {
private:
    int _start;
    int _end;
    double _length;

public:
    int getNodeStart() const;

    void setNodeStart(int start);

    int getNodeEnd() const;

    void setNodeEnd(int end);

    double getLength() const;

    void setLength(double d);

    Edge(int start, int end, double length);
};


#endif //GREEDYALGORITHM_EDGE_H
