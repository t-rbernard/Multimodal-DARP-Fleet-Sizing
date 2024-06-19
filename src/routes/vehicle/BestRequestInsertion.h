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
    const SAEVKeyPoint* _originInsertionKP;
    const SAEVKeyPoint* _destinationInsertionKP;
    double _score{ std::numeric_limits<double>::max() };

public:
    /**
     * Default constructor to allow pre-sizing containers
     */
    BestRequestInsertion() {};
    /**
     * Constructs a BestRequestInsertion object, containing Origin/Destination insertion indexes and the score associated
     * @param originInsertionKP Index of insertion for the origin of the request in the route
     * @param destinationInsertionKP Index of insertion for the destination of the request in the route
     * @param score
     */
    BestRequestInsertion(const SAEVKeyPoint *originInsertionKP, const SAEVKeyPoint *destinationInsertionKP, double score)
            : _originInsertionKP(originInsertionKP), _destinationInsertionKP(destinationInsertionKP),
              _score(score) {};

    [[nodiscard]] const SAEVKeyPoint *getOriginInsertionKp() const {
        return _originInsertionKP;
    }

    void setOriginInsertionKp(SAEVKeyPoint *originInsertionKp) {
        _originInsertionKP = originInsertionKp;
    }

    [[nodiscard]] const SAEVKeyPoint *getDestinationInsertionKp() const {
        return _destinationInsertionKP;
    }

    void setDestinationInsertionKp(SAEVKeyPoint *destinationInsertionKp) {
        _destinationInsertionKP = destinationInsertionKp;
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
