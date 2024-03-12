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
    explicit TransitStateContainer(int size)
    {
        solutionVector.resize(size);
        for(size_t i = 0; i < size; ++i) {
            solutionVector.at(i).emplace_back();
            solutionVector.at(i).emplace_back();
        }
    }
    explicit TransitStateContainer(size_t size)
    {
        //Reserve space for all nodes and add empty solutions to reserve memory
        solutionVector.resize(size);
        for(size_t i = 0; i < size; ++i) {
            solutionVector.at(i).emplace_back();
            solutionVector.at(i).emplace_back();
        }
    }
    /**
     * Returns the current best solution for the given node index
     * @param nodeIndex
     * @return The first solution of potentially two saved in this array
     */
    std::vector<TransitAlgorithmState>& getBestSolutions(int nodeIndex){ return solutionVector.at(nodeIndex);}
    TransitAlgorithmState& getBestSolution(int nodeIndex, int nbConnections){ return solutionVector.at(nodeIndex).at(nbConnections == 0 ? 0 : nbConnections - 1);}
    std::vector<TransitAlgorithmState>& getSolutions(int nodeIndex){ return solutionVector.at(nodeIndex);}
    std::vector<std::vector<TransitAlgorithmState>>& getSolutionsVector(){ return solutionVector;}

    bool strictlyDominates(const TransitAlgorithmState& state){
        if(state.getNodeIndex() != -1)
            return getBestSolution(state.getNodeIndex(), state.getNbConnections()).strictlyDominates(state);
        else
            return true;
    }


    /**
     * Resizes solution vector and initializes state vectors to be able to add our solutions during algorithm execution
     * @param nbNodes How many nodes there are in the full graph
     */
    void resizeSolutionsVector(int nbNodes){ solutionVector.resize(nbNodes);}


    void addNewState(int nodeIndex, const TransitAlgorithmState& newState)
    {
        if(newState.getNbConnections() > 0) {
            solutionVector.at(nodeIndex).at(newState.getNbConnections() - 1) = newState;
        }
    }

    void pushEmptyState(int nodeIndex)
    {
        TransitAlgorithmState newState = TransitAlgorithmState(nodeIndex);
        solutionVector.at(nodeIndex).emplace_back(newState);
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
