//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
#define GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H

#include <vector>
#include "TransitShortestPath.h"

class TransitStateContainer {
private:
    //    int x,delta;
    std::vector<std::vector<TransitAlgorithmState>>  solutionVector;

public:
    /**
     * Returns the current best solution for the given node index
     * @param nodeIndex
     * @return The first solution of potentially two saved in this array
     */
    TransitAlgorithmState& getBestSolution(int nodeIndex){ return solutionVector.at(nodeIndex)[0];}
    std::vector<TransitAlgorithmState>& getSolutions(int nodeIndex){ return solutionVector.at(nodeIndex);}
    std::vector<std::vector<TransitAlgorithmState>>& getSolutionsVector(){ return solutionVector;}

    /**
     * Resizes solution vector and initializes state vectors to be able to add our solutions during algorithm execution
     * @param nbNodes How many nodes there are in the full graph
     */
    void resizeSolutionsVector(int nbNodes){ solutionVector.resize(nbNodes);}


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

    void pushEmptyState(int nodeIndex)
    {
        TransitAlgorithmState newState = TransitAlgorithmState(nodeIndex, INT16_MAX, 0);
        solutionVector.at(nodeIndex).emplace_back(newState);
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
