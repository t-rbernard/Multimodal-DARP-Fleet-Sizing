//
// Created by Bebs on 20/02/2024.
//

#ifndef GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
#define GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H

#include <vector>
#include "TransitShortestPath.h"

class TransitShortestPathIterationSolutionContainer {
//    int x,delta;
    std::vector<std::array<TransitAlgorithmState,2>>  solutionVector;

    /**
     * Check if the two solutions for the given nodeIndex are ordered properly and swaps them if needed
     * @param nodeIndex
     */
    void maintainShortestPathOrder(int nodeIndex)
    {
        std::array<TransitAlgorithmState,2> array = solutionVector.at(nodeIndex);
        if(array[0] > array[1]) {
            TransitAlgorithmState tmp = array[1];
            array[1] = array[0];
            array[0] = tmp;
        }
    }

    bool tryAddNewState(int nodeIndex, const TransitAlgorithmState& newState)
    {
        if(newState.strictlyDominates(solutionVector.at(nodeIndex)[0])) {
            solutionVector.at(nodeIndex)[0] = newState;
            return true;
        } else if(newState.strictlyDominates(solutionVector.at(nodeIndex)[1])) {
            solutionVector.at(nodeIndex)[1] = newState;
            return true;
        } else {
            return false;
        }
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
