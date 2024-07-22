//
// Created by romain on 16/07/24.
//

#ifndef GREEDYALGORITHM_MATRIXSHORTESTPATHCONTAINER_H
#define GREEDYALGORITHM_MATRIXSHORTESTPATHCONTAINER_H


#include <vector>
#include <cstdint>
#include <algorithm>

class MatrixShortestPathContainer {
private:
    std::vector<std::vector<uint>> _distanceMatrix{};
public:
    MatrixShortestPathContainer() = default;

    /**
     * Moves the given shortestPathsMatrix to initialize the container with the given distances.
     * @param shortestPathsMatrix
     */
    explicit MatrixShortestPathContainer(std::vector<std::vector<uint>> shortestPathsMatrix) : _distanceMatrix(std::move(shortestPathsMatrix)) {}

    explicit MatrixShortestPathContainer(size_t matrixSize) {
        _distanceMatrix.resize(matrixSize);
        for(size_t i = 0; i < _distanceMatrix.capacity(); ++i)
            _distanceMatrix[i].resize(matrixSize, INT16_MAX);
    }

    [[nodiscard]] const std::vector<std::vector<uint>> &getDistanceMatrix() const {
        return _distanceMatrix;
    }

    /**
     * Replaces all the results for the given starting node with the given results vector.
     * the whole vector is cleared before moving the results vector data
     * @param startingNodeIdx starting node of the paths in our results/destination distance vector
     * @param results a vector which will be moved to this current results collection
     */
    void replaceResultsCollection(size_t startingNodeIdx, std::vector<uint>& results) {
        _distanceMatrix[startingNodeIdx].clear();
        std::ranges::move(results, _distanceMatrix[startingNodeIdx].begin());
    }

    /**
     * Gives the shortest path time from a given node index to a given destination node index
     * @param from origin node index
     * @param to destination node index
     * @return the distance of the shortest path between those two nodes
     */
    uint getShortestPath(size_t from, size_t to) { return _distanceMatrix[from][to]; }

    /**
     *
     * @param idx Node index from
     * @return The collection of distances to get to every graph node from the given index
     */
    std::vector<uint>& operator[](size_t idx) { return _distanceMatrix[idx]; }
};


#endif //GREEDYALGORITHM_MATRIXSHORTESTPATHCONTAINER_H
