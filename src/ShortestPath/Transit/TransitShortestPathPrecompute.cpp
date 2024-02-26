//
// Created by rbernard on 20/02/2024.
//

#include "TransitShortestPathPrecompute.h"
#include "TransitStateContainer.h"
#include <queue>

//TODO:
//  Tests :
//      - priority queue order
//      - Reference value still valid after popping (otherwise, pop at the end, before adding new state)
TransitStateContainer TransitShortestPathPrecompute::executeAlgorithm(const Graph& graph, int nodeIndex, int instant) {
    TransitStateContainer solutionsContainer{};
    solutionsContainer.resizeSolutionsVector(graph.getNodesVector().size());
    for(nodeIndex = 0; nodeIndex < graph.getNodesVector().size(); ++nodeIndex) {
        solutionsContainer.pushEmptyState(nodeIndex);
    }

    std::priority_queue<TransitAlgorithmState> statePriorityQueue;
    statePriorityQueue.emplace(nodeIndex, instant,0);

    TransitAlgorithmState currentState;
    while(!statePriorityQueue.empty())
    {
        //Extract head from our state priority queue
        currentState = statePriorityQueue.top();
        statePriorityQueue.pop();
        if(!solutionsContainer.getBestSolution(currentState.getNodeIndex()).strictlyDominates(currentState)) {
            for (auto& lineStop : graph.getNode(currentState.getNodeIndex()).getPTLinesSet())
            {
                int nextNode = lineStop.getNextNodeIndex();
                //If there is a proper next node and if it's not the same as our last used line stop predecessor
                if(nextNode != -1 && nextNode != currentState.getConnections().back().getPrecedingNodeIndex()) {
                    if(currentState.getConnections().back().getLineRef() != lineStop.getLineRef())
                    {
                        //TODO: extend
                    }
                    //TODO: check dominance of extended state
                    //  Add to the lists
                }
            }
        }
    }

    //TODO: maybe finalise our container
    //  Solution formatting should be done elsewhere for isolation purposes

    return solutionsContainer;
}
