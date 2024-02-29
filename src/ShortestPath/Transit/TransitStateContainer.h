//
// Created by rbernard on 20/02/2024.
//

#ifndef GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
#define GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H

#include <vector>
#include "TransitShortestPath.h"

#ifdef DEBUG_TRANSIT_PRECOMPUTE
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

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
        DEBUG_MSG("Trying to add state " + newState.toString());
        if(!solutionVector.at(nodeIndex).empty() && newState.strictlyDominates(solutionVector.at(nodeIndex)[0])) {
            DEBUG_MSG("Added state to position 0, replacing " + solutionVector.at(nodeIndex)[0].toString());
            solutionVector.at(nodeIndex)[0] = newState;
            return true;
        } else if(solutionVector.at(nodeIndex).size() > 1 && newState.strictlyDominates(solutionVector.at(nodeIndex)[1])) {
            DEBUG_MSG("Added state to position 1, replacing " + solutionVector.at(nodeIndex)[1].toString());
            solutionVector.at(nodeIndex)[1] = newState;
            return true;
        } else {
            DEBUG_MSG("State wasn't added to the container because it doesn't dominate \n" +
                        solutionVector.at(nodeIndex)[0].toString() +
                        (solutionVector.at(nodeIndex).size() > 1 ? "\nor " + solutionVector.at(nodeIndex)[1].toString() : ""));
            return false;
        }
    }

    void pushEmptyState(int nodeIndex)
    {
        TransitAlgorithmState newState = TransitAlgorithmState(nodeIndex);
        solutionVector.at(nodeIndex).emplace_back(newState);
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
