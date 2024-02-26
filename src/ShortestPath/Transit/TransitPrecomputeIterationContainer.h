//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
#define GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H

#include <vector>
#include "TransitShortestPath.h"

class TransitPrecomputeIterationContainer {
//    int x,delta;
    std::vector<std::array<TransitAlgorithmState,2>>  solutionVector;

    /**
     * Returns the current best solution for the given node index
     * @param nodeIndex
     * @return The first solution of potentially two saved in this array
     */
    TransitAlgorithmState getBestSolution(int nodeIndex){ return solutionVector.at(nodeIndex)[0];}
    std::array<TransitAlgorithmState,2> getSolutions(int nodeIndex){ return solutionVector.at(nodeIndex);}

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

    void finalizeSolutions()
    {
        for(auto& solution : this->solutionVector)
        {
            //TODO: check if there's a better solution not involving recreating everything (tbh "finalizing" isn't exactly required if we just take first solution every time
            solution.at(1) = solution.at(0); //just replace the second useless solution with a reference to the first solution
        }
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
