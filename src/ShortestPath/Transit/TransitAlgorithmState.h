//
// Created by romain on 21/02/24.
//

#ifndef GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
#define GREEDYALGORITHM_TRANSITALGORITHMSTATE_H


#include <array>
#include "../../instance/graph/LineStop.h"

class TransitAlgorithmState {
private:
    int _nodeIndex;
    int _instant;
    int _passageIndex;
    std::vector<LineStop> _connections;

public:
    TransitAlgorithmState(int currentNode, int currentInstant, int currentPassageIndex) {
        _nodeIndex = currentNode;
        _instant = currentInstant;
        _passageIndex = currentPassageIndex;
        _connections.reserve(2); //TODO : replace constant max amount of connexions with a global parameter
    }

    TransitAlgorithmState(TransitAlgorithmState& baseState) {
        _nodeIndex = baseState.getNodeIndex();
        _instant = baseState.getInstant();
        _passageIndex = baseState.getPassageIndex();

        //Copy old connections
        for(auto& lineStop : baseState.getConnections()) {
            _connections.emplace_back(lineStop);
        }
    }

    TransitAlgorithmState(TransitAlgorithmState&& baseStatePointer) = default;

    TransitAlgorithmState(TransitAlgorithmState& baseState, const LineStop& newConnection) {
        _nodeIndex = baseState.getNodeIndex();
        _instant = baseState.getInstant();
        _passageIndex = baseState.getPassageIndex();

        //Copy old connections
        for(auto& lineStop : baseState.getConnections()) {
            _connections.emplace_back(lineStop);
        }

        addNewConnection(newConnection);
    }

    explicit TransitAlgorithmState(int nodeIndex) {
        _nodeIndex = nodeIndex;
        _instant = INT16_MAX;
        _passageIndex = -1;
    }

    explicit TransitAlgorithmState() {
        _nodeIndex = -1;
        _instant = INT16_MAX;
        _passageIndex = -1;
    }

    [[nodiscard]] int getNodeIndex() const {
        return _nodeIndex;
    }

    [[nodiscard]] int getInstant() const {
        return _instant;
    }

    [[nodiscard]] int getPassageIndex() const {
        return _passageIndex;
    }

    [[nodiscard]] const std::vector<LineStop> &getConnections() const {
        return _connections;
    }

    void setNodeIndex(int nodeIndex) {
        _nodeIndex = nodeIndex;
    }

    void setInstant(int instant) {
        _instant = instant;
    }

    void setPassageIndex(int passageIndex) {
        _passageIndex = passageIndex;
    }

    /**
     *
     * @param connection
     * @return
     */
    bool addNewConnection(const LineStop& connection)
    {
        if(_connections.size() < _connections.max_size()) {
            _connections.emplace_back(connection);
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
        return this->_nodeIndex == rhs.getNodeIndex() //same current node
               && ((this->getInstant() < rhs.getInstant() && (this->getConnections().size() <= rhs.getConnections().size() || rhs.getConnections().empty()))
               || (this->getInstant() < rhs.getInstant() && this->getConnections().size() == rhs.getConnections().size()));
    }

    /**
     * States are ordered by their best arrival time first, and if equal, by the amount of connections to decide which state to extend first
     * @param rhs
     * @return
     */
    bool operator<(const TransitAlgorithmState& rhs) const {
        return this->_nodeIndex == rhs.getNodeIndex() //same current node
                && (this->getInstant() < rhs.getInstant() //strictly better time
                || (this->getInstant() == rhs.getInstant()
                    && this->getConnections().size() < rhs.getConnections().size())); //same arrival time and strictly better connections
    }

    bool operator>(const TransitAlgorithmState& rhs) const {
        return this->_nodeIndex == rhs.getNodeIndex() //same current node
                && (this->getInstant() > rhs.getInstant()
                || (this->getInstant() == rhs.getInstant()
                    && this->getConnections().size() > rhs.getConnections().size()));
    }

    TransitAlgorithmState& operator=(const TransitAlgorithmState& baseState) {
        _nodeIndex = baseState.getNodeIndex();
        _instant = baseState.getInstant();
        _passageIndex = baseState.getPassageIndex();
        //Copy old connections
        for(auto& lineStop : baseState.getConnections()) {
            _connections.emplace_back(lineStop);
        }

        return *this;
    }

};


#endif //GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
