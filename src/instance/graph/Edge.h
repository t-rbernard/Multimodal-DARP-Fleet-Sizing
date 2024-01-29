//
// Created by romain on 22/01/24.
//

#ifndef GREEDYALGORITHM_EDGE_H
#define GREEDYALGORITHM_EDGE_H


#include "Node.h"

class Edge {
private:
    Node* in;
    Node* out;
    double length;

public:
    Node *getIn() const;

    void setIn(Node *in);

    Node *getOut() const;

    void setOut(Node *out);

    double getLength() const;

    void setLength(double d);
};


#endif //GREEDYALGORITHM_EDGE_H
