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
    std::vector<TransitAlgorithmState>  solutionVector;

public:
    /**
     * Returns the current best solution for the given node index
     * @param nodeIndex
     * @return The first solution of potentially two saved in this array
     */
    TransitAlgorithmState getBestSolution(int nodeIndex){ return solutionVector.at(nodeIndex);}
    std::vector<TransitAlgorithmState>& getSolutionsVector(){ return solutionVector;}
//    void resizeSolutionsVector(int nbNodes){ solutionVector.resize(nbNodes);}


    bool tryAddNewState(int nodeIndex, const TransitAlgorithmState& newState)
    {
        if(newState.strictlyDominates(solutionVector.at(nodeIndex))) {
            solutionVector.at(nodeIndex) = newState;
            return true;
        } else {
            return false;
        }
    }

    void pushNewState(int nodeIndex)
    {
        TransitAlgorithmState newState = TransitAlgorithmState(nodeIndex, 0, 0);
        solutionVector.emplace_back(&newState);
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
