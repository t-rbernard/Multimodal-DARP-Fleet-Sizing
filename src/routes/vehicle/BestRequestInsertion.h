//
// Created by romain on 18/06/24.
//

#ifndef GREEDYALGORITHM_BESTREQUESTINSERTION_H
#define GREEDYALGORITHM_BESTREQUESTINSERTION_H


#include <cstddef>
#include <compare>
#include <cstdint>
#include <limits>

class BestRequestInsertion {
private:
    size_t _originInsertionIndex{std::numeric_limits<size_t>::max()};
    size_t _destinationInsertionIndex{std::numeric_limits<size_t>::max()};
    double _score{ std::numeric_limits<double>::max() };

public:
    /**
     * Default constructor to allow pre-sizing containers
     */
    BestRequestInsertion() {};
    /**
     * Constructs a BestRequestInsertion object, containing Origin/Destination insertion indexes and the score associated
     * @param originInsertionIndex Index of insertion for the origin of the request in the route
     * @param destinationInsertionIndex Index of insertion for the destination of the request in the route
     * @param score
     */
    BestRequestInsertion(size_t originInsertionIndex, size_t destinationInsertionIndex, double score)
            : _originInsertionIndex(originInsertionIndex), _destinationInsertionIndex(destinationInsertionIndex),
              _score(score) {};

    [[nodiscard]] size_t getOriginInsertionIndex() const {
        return _originInsertionIndex;
    }

    void setOriginInsertionIndex(size_t originInsertionIndex) {
        BestRequestInsertion::_originInsertionIndex = originInsertionIndex;
    }

    [[nodiscard]] size_t getDestinationInsertionIndex() const {
        return _destinationInsertionIndex;
    }

    void setDestinationInsertionIndex(size_t destinationInsertionIndex) {
        BestRequestInsertion::_destinationInsertionIndex = destinationInsertionIndex;
    }

    [[nodiscard]] double getScore() const {
        return _score;
    }

    void setScore(double score) {
        BestRequestInsertion::_score = score;
    }

    std::partial_ordering operator<=>(const BestRequestInsertion &rhs) const {
        return _score <=> rhs._score;
    }
};


#endif //GREEDYALGORITHM_BESTREQUESTINSERTION_H
