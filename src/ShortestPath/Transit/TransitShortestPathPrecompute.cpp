//
// Created by rbernard on 20/02/2024.
//

#include "TransitShortestPathPrecompute.h"
#include "TransitStateContainer.h"
#include <queue>

#ifdef DEBUG_TRANSIT_PRECOMPUTE
#include <iostream>
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

//TODO:
//  Tests :
//      - priority queue order
//      - Reference value still valid after popping (otherwise, pop at the end, before adding new state)
TransitStateContainer TransitShortestPathPrecompute::executeAlgorithm(const Graph& graph, int nodeIndex, int instant) {
    //Init container, priority queue and state variables
    TransitStateContainer solutionsContainer{graph.getNodesVector().size()};
    std::priority_queue<TransitAlgorithmState> statePriorityQueue;
    statePriorityQueue.emplace(nodeIndex, instant,0);

    TransitAlgorithmState currentState;
    while(!statePriorityQueue.empty())
    {
        //Extract head from our state priority queue
        currentState = statePriorityQueue.top();
        statePriorityQueue.pop();
        if(!solutionsContainer.strictlyDominates(currentState)) {
            DEBUG_MSG("\n\nComparing state " + currentState.toString() + " and " + solutionsContainer.getBestSolution(currentState.getNodeIndex(), currentState.getNbConnections()).toString());
            for (auto& lineStop : graph.getPTLinesSet(currentState.getNodeIndex()))
            {
                int nextNode = lineStop.getNextNodeIndex();
                //If there is a proper next node and if it's not the same as our last used line stop predecessor
                if(nextNode != -1 && (currentState.isEmpty() || nextNode != currentState.getPrecedingNodeIndex())) {
                    DEBUG_MSG("Extension from line " + lineStop.getLineRef().getLineId() + " towards node " + std::to_string(nextNode));
                    TransitAlgorithmState newState; //define variable before conditionals
                    if(currentState.isEmpty() || currentState.getLastConnectionLine() != lineStop.getLineRef()) // if new line is different than current line
                    {
                        if(currentState.canAddConnection()) {
                            int nextPassageIndex = lineStop.getLineRef().findNextScheduledPassage(
                                    lineStop.getStopIndex(), currentState.getInstant());
                            if (nextPassageIndex == lineStop.getLineRef().scheduleSize()) {
                                newState.setInstant(INT16_MAX);
                            } else {
                                newState = TransitAlgorithmState(currentState, lineStop);
                                newState.setNodeIndex(nextNode);
                                newState.setPassageIndex(nextPassageIndex); //get next passage for new line
                                newState.setInstant(lineStop.getInstant(lineStop.getStopIndex() + 1,
                                                                        nextPassageIndex)); //replace time with arrival time on next node
                            }
                        }
                    } else {
                        newState = TransitAlgorithmState(currentState);
                        newState.setNodeIndex(nextNode);
                        newState.setPassageIndex(currentState.getPassageIndex()); //get next passage for new line
                        newState.setInstant(lineStop.getLineRef().getInstant(lineStop.getStopIndex() + 1, currentState.getPassageIndex())); //replace time with
                    }

                    DEBUG_MSG("Created new state " + newState.toString());

                    //Add new state to the solution container and the priority queue if it's not strictly dominated by an existing solution
                    if(!solutionsContainer.strictlyDominates(currentState)) {
                        DEBUG_MSG("Candidate state " + newState.toString() + " is being added to solution container and priority queue");
                        DEBUG_MSG("Candidate state " + newState.toString() + " is being added to solution container and priority queue\n");
                        solutionsContainer.addNewState(nextNode, newState);
                        statePriorityQueue.emplace(newState);
                    }
                }
            }
            DEBUG_MSG("Done extending state " + currentState.toString());
        }
    }

    //TODO: maybe finalise our container
    //  Solution formatting should be done elsewhere for isolation purposes

    return solutionsContainer;
}
