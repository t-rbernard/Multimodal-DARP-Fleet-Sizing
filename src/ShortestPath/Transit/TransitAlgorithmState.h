//
// Created by romain on 21/02/24.
//

#ifndef GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
#define GREEDYALGORITHM_TRANSITALGORITHMSTATE_H


#include <array>
#include <cstdint>
#include "../../instance/graph/LineStop.h"
#include "../../utils/Constants.h"

class TransitAlgorithmState {
private:
    int _nodeIndex;
    int _instant{INT16_MAX};
    int _passageIndex{-1};
    std::vector<std::reference_wrapper<const LineStop>> _connections;//{Constants::MAX_TRANSIT_CONNECTIONS};
    int _precedingNodeIndex{-1};

public:
    TransitAlgorithmState(int currentNode, int currentInstant, int currentPassageIndex, int precedingNodeIndex) :
            _nodeIndex(currentNode), _instant(currentInstant),
            _passageIndex(currentPassageIndex), _precedingNodeIndex(precedingNodeIndex) {
        _connections.reserve(Constants::MAX_TRANSIT_CONNECTIONS);
    }

    TransitAlgorithmState(TransitAlgorithmState const& baseState) :
    _nodeIndex(baseState.getNodeIndex()), _instant(baseState.getInstant()),
    _passageIndex(baseState.getPassageIndex()), _precedingNodeIndex(baseState.getPrecedingNodeIndex()) {
        //Copy old connections
        _connections.clear();
        _connections.reserve(Constants::MAX_TRANSIT_CONNECTIONS);
        for(const auto& lineStop : baseState.getConnections()) {
            _connections.emplace_back(lineStop);
        }
    }

    TransitAlgorithmState(TransitAlgorithmState const& baseState, const LineStop& newConnection) :
    _nodeIndex(baseState.getNodeIndex()), _instant(baseState.getInstant()),
    _passageIndex(baseState.getPassageIndex()), _precedingNodeIndex(baseState.getPrecedingNodeIndex()) {
        //Copy old connections
        _connections.clear();
        _connections.reserve(Constants::MAX_TRANSIT_CONNECTIONS);
        for(const auto& lineStop : baseState.getConnections()) {
            _connections.emplace_back(lineStop);
        }

        addNewConnection(newConnection);
    }

    explicit TransitAlgorithmState(int nodeIndex) : _nodeIndex(nodeIndex) {
        _connections.reserve(Constants::MAX_TRANSIT_CONNECTIONS);
    }

    explicit TransitAlgorithmState() : _nodeIndex(-1) {
        _connections.reserve(Constants::MAX_TRANSIT_CONNECTIONS);
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

    [[nodiscard]] const std::vector<std::reference_wrapper<const LineStop>> &getConnections() const {
        return _connections;
    }

    [[nodiscard]] bool isEmpty() const {
        return _connections.empty();
    }

    [[nodiscard]] bool canAddConnection() const {
        return _connections.size() < Constants::MAX_TRANSIT_CONNECTIONS;
    }

    [[nodiscard]] size_t getNbConnections() const {
        return _connections.size();
    }

    [[nodiscard]] const LineStop& getLastConnectionLineStop() const {
        return _connections.back();
    }

    [[nodiscard]] const Line& getLastConnectionLine() const {
        return _connections.back().get().getLineRef();
    }

    [[nodiscard]] int getPrecedingNodeIndex() const {
        return _precedingNodeIndex;
    }

    void setPrecedingNodeIndex(int nodeIndex) {
        _precedingNodeIndex = nodeIndex;
    }

    [[nodiscard]] int getNextNodeIndex() const {
        return _connections.back().get().getNextNodeIndex();
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

    /**
     * Strict dominance between two transit shortest path states happens if state *this* has :
     * 1) lower amount of connections and same arrival time OR 2) lower arrival time than the state rhs
     * @param rhs
     * @return
     */
    [[nodiscard]] bool strictlyDominates(const TransitAlgorithmState& rhs) const {
        return this->getNodeIndex() == rhs.getNodeIndex()
        && this->getLastConnectionLine().getLineId() == rhs.getLastConnectionLine().getLineId()
//        TODO : check  && (this->getLastConnectionLine() == rhs.getLastConnectionLine() || this->getNbConnections() == Constants::MAX_TRANSIT_CONNECTIONS)
        && ((this->getInstant() < rhs.getInstant() && this->getConnections().size() <= rhs.getConnections().size())
        || (this->getInstant() == rhs.getInstant() && this->getConnections().size() < rhs.getConnections().size()));
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

    bool operator==(const TransitAlgorithmState& rhs) const {
        return this->_nodeIndex == rhs.getNodeIndex() //same current node
                && this->getInstant() == rhs.getInstant()
                && this->getConnections().size() == rhs.getConnections().size();
    }

    bool operator!=(const TransitAlgorithmState& rhs) const {
        return this->_nodeIndex != rhs.getNodeIndex() //same current node
                || this->getInstant() != rhs.getInstant()
                || this->getConnections().size() != rhs.getConnections().size();
    }

    [[nodiscard]] std::string toString() const {
        std::string res = "Node: " + std::to_string(_nodeIndex) + ", Instant: " + std::to_string(_instant);

        //Add line names in order if needed
        if(!_connections.empty()) {
            res += ", Connections: ";
            if(_connections.size() > 1) {
                for(int i = 0; i < _connections.size() - 1; ++i) {
                    res += _connections[i].get().getLineRef().getLineId() + " -> ";
                }
            }

            res += _connections[_connections.size() - 1].get().getLineRef().getLineId();

        }

        return res;
    }

};


#endif //GREEDYALGORITHM_TRANSITALGORITHMSTATE_H
