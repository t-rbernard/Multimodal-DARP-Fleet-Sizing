//
// Created by romain on 21/02/24.
//

#ifndef GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
#define GREEDYALGORITHM_TRANSITALGORITHMSTATE_H


#include <array>
#include "../../instance/graph/LineStop.h"

class TransitAlgorithmState {
private:
    int _currentNodeIndex;
    int _currentInstant;
    int _currentPassageIndex;
    std::vector<LineStop> _connections;

public:
    TransitAlgorithmState(int currentNode, int currentInstant, int currentPassageIndex) {
        _currentNodeIndex = currentNode;
        _currentInstant = currentInstant;
        _currentPassageIndex = currentPassageIndex;
        _connections.reserve(2); //TODO : replace constant max amount of connexions with a global parameter
    }

    TransitAlgorithmState(TransitAlgorithmState& baseState) {
        _currentNodeIndex = baseState.getCurrentNodeIndex();
        _currentInstant = baseState.getCurrentInstant();
        _currentPassageIndex = baseState.getCurrentPassageIndex();

        if(!baseState.getConnections().empty() && (!baseState.getConnections().empty() && !baseState.getConnections().at(0).isEmpty()))
            std::copy(baseState.getConnections().begin(), baseState.getConnections().end(), _connections.begin());
    }

    TransitAlgorithmState(TransitAlgorithmState&& baseStatePointer) = default;

    TransitAlgorithmState(TransitAlgorithmState& baseState, const LineStop& newConnection) {
        _currentNodeIndex = baseState.getCurrentNodeIndex();
        _currentInstant = baseState.getCurrentInstant();
        _currentPassageIndex = baseState.getCurrentPassageIndex();

        if(!baseState.getConnections().empty() && (!baseState.getConnections().empty() && !baseState.getConnections().at(0).isEmpty()))
            std::copy(baseState.getConnections().begin(), baseState.getConnections().end(), _connections.begin());

        addNewConnection(newConnection);
    }

    explicit TransitAlgorithmState(int nodeIndex) {
        _currentNodeIndex = nodeIndex;
        _currentInstant = 0;
        _currentPassageIndex = 0;
    }

    explicit TransitAlgorithmState() {
        _currentNodeIndex = -1;
        _currentInstant = 0;
        _currentPassageIndex = 0;
    }

    [[nodiscard]] int getCurrentNodeIndex() const {
        return _currentNodeIndex;
    }

    [[nodiscard]] int getCurrentInstant() const {
        return _currentInstant;
    }

    [[nodiscard]] int getCurrentPassageIndex() const {
        return _currentPassageIndex;
    }

    [[nodiscard]] const std::vector<LineStop> &getConnections() const {
        return _connections;
    }

    /**
     *
     * @param connection
     * @return
     */
    bool addNewConnection(const LineStop& connection)
    {
        if(_connections.size() < _connections.max_size()) {
            _connections[_connections.size()] = connection;
            return true;
        } else {
            return false;
        }
    }

    bool canAddNewConnection() {return _connections.size() < _connections.max_size(); }

    /**
     * Strict dominance between two transit shortest path states happens if state *this* has :
     * 1) lower amount of connections and same arrival time OR 2) lower arrival time than the state rhs
     * @param rhs
     * @return
     */
    [[nodiscard]] bool strictlyDominates(const TransitAlgorithmState& rhs) const {
        return this->_currentNodeIndex == rhs.getCurrentNodeIndex() //same current node
               && ((this->getCurrentInstant() < rhs.getCurrentInstant() && this->getConnections().size() <= rhs.getConnections().size())
               || (this->getCurrentInstant() < rhs.getCurrentInstant() && this->getConnections().size() == rhs.getConnections().size()));
    }

    /**
     * States are ordered by their best arrival time first, and if equal, by the amount of connections to decide which state to extend first
     * @param rhs
     * @return
     */
    bool operator<(const TransitAlgorithmState& rhs) const {
        return this->_currentNodeIndex == rhs.getCurrentNodeIndex() //same current node
                && (this->getCurrentInstant() < rhs.getCurrentInstant() //strictly better time
                || (this->getCurrentInstant() == rhs.getCurrentInstant()
                    && this->getConnections().size() < rhs.getConnections().size())); //same arrival time and strictly better connections
    }

    bool operator>(const TransitAlgorithmState& rhs) const {
        return this->_currentNodeIndex == rhs.getCurrentNodeIndex() //same current node
                && (this->getCurrentInstant() > rhs.getCurrentInstant()
                || (this->getCurrentInstant() == rhs.getCurrentInstant()
                    && this->getConnections().size() > rhs.getConnections().size()));
    }

    TransitAlgorithmState& operator=(const TransitAlgorithmState& baseState) {
        _currentNodeIndex = baseState.getCurrentNodeIndex();
        _currentInstant = baseState.getCurrentInstant();
        _currentPassageIndex = baseState.getCurrentPassageIndex();
        std::copy(baseState.getConnections().begin(), baseState.getConnections().end(), _connections.begin());

        return *this;
    }

};


#endif //GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
