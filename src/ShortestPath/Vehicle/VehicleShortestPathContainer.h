//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_VEHICLESHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_VEHICLESHORTESTPATHCONTAINER_H


#include <vector>
#include <cstdint>
#include <algorithm>

class VehicleShortestPathContainer {
private:
    std::vector<std::vector<uint>> _distanceMatrix{};
public:
    VehicleShortestPathContainer() = default;
    VehicleShortestPathContainer(size_t matrixSize) {
        _distanceMatrix.resize(matrixSize);
        for(int i = 0; i < _distanceMatrix.capacity(); ++i)
            _distanceMatrix[i].resize(matrixSize, INT16_MAX);
    }

    [[nodiscard]] const std::vector<std::vector<uint>> &getDistanceMatrix() const {
        return _distanceMatrix;
    }

    /**
     * Replaces all the results for the given starting node with the given results vector.
     * the whole vector is cleared before
     * @param startingNode
     * @param results
     */
    void replaceResultsCollection(size_t startingNode, std::vector<uint>& results) {
        _distanceMatrix[startingNode].clear();
        std::ranges::move(results, _distanceMatrix[startingNode].begin());
    }

    std::vector<uint>& operator[](size_t idx) { return _distanceMatrix[idx]; }
};


#endif //GREEDYALGORITHM_VEHICLESHORTESTPATHCONTAINER_H
