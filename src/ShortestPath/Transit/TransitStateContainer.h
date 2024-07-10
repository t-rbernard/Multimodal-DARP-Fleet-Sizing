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
            solutionVector[i].emplace_back();
            solutionVector[i].emplace_back();
        }
    }
    explicit TransitStateContainer(size_t size)
    {
        //Reserve space for all nodes and add empty solutions to reserve memory
        solutionVector.resize(size);
        for(size_t i = 0; i < size; ++i) {
            solutionVector[i].emplace_back();
            solutionVector[i].emplace_back();
        }
    }
    /**
     * Returns the current best solution for the given node index
     * @param nodeIndex
     * @return The first solution of potentially two saved in this array
     */
    [[nodiscard]] const TransitAlgorithmState& getBestSolution(int nodeIndex) const{
        if(solutionVector[1] < solutionVector[0])
            return solutionVector[nodeIndex][1];
        else
            return solutionVector[nodeIndex][0];
    }

    TransitAlgorithmState& getBestSolution(int nodeIndex, int nbConnections){ return solutionVector[nodeIndex][nbConnections == 0 ? 0 : nbConnections - 1];}
    std::vector<TransitAlgorithmState>& getSolutions(int nodeIndex){ return solutionVector[nodeIndex];}
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


    void replaceBestSolutionIfNecessary(int nodeIndex, const TransitAlgorithmState& newState)
    {
        if(newState.getNbConnections() > 0) {
            TransitAlgorithmState const& currentBest = solutionVector[nodeIndex][newState.getNbConnections() - 1];
            if(currentBest.getInstant() > newState.getInstant()) {
                DEBUG_MSG("Candidate state " + newState.toString() + " is the new fastest solution");
                solutionVector[nodeIndex][newState.getNbConnections() - 1] = newState;
            }
        }
    }

    /**
     * Compares the states available to get to a given node index and returns the best by comparing their time of arrival.
     * If equivalent solutions wrt time exist, the one with the lowest amount of connections required will be returned
     * @param nodeIndex The node we try to get to
     * @return
     */
    TransitAlgorithmState getBestSolution(int nodeIndex) {
        TransitAlgorithmState currentBestSol = solutionVector[nodeIndex][0];
        for(size_t i = 1; i < solutionVector[nodeIndex].size(); ++i) {
            if(solutionVector[nodeIndex][i].getInstant() < currentBestSol.getInstant()) {
                currentBestSol = solutionVector[nodeIndex][i];
            }
        }

        return currentBestSol;
    }

    void pushEmptyState(int nodeIndex)
    {
        TransitAlgorithmState newState = TransitAlgorithmState(nodeIndex);
        solutionVector[nodeIndex].emplace_back(newState);
    }
};


#endif //GREEDYALGORITHM_ALGORITHMITERATIONSOLUTIONCONTAINER_H
