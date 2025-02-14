//
// Created by romain on 20/02/24.
//

#ifndef GREEDYALGORITHM_SEARCHALGORITHMS_H
#define GREEDYALGORITHM_SEARCHALGORITHMS_H


#include <vector>

template <typename Comparable>
class SearchAlgorithms {
public:
    /**
     * Returns the first index corresponding exactly to the given value in the
     * @param sortedVector a sorted vector in which to search for a value. If the vector isn't sorted, there is no guarantee that we return the appropriate index or that performance is O(log n)
     * @param value the comparable value we are looking for in the vector
     * @return the size of the vector if all vector values are lower than our search, the index of the first larger or equal value otherwise
     */
    static size_t findNextSortedValue(std::vector<Comparable> sortedVector, Comparable value) {
        auto iterator = std::lower_bound(sortedVector.begin(), sortedVector.end(), value);
        return std::distance(sortedVector.begin(), iterator);
    }

    /**
     * Returns the first index corresponding exactly to the given value in the vector
     * @param sortedVector a sorted vector in which to search for a value. If the vector isn't sorted, there is no guarantee that we return the first index or that performance is O(log n)
     * @param value the comparable value we are looking for in the vector
     * @return -1 if the exact value hasn't been found or the index of the value if it's been found
     */
    static size_t vectorBinarySearch(std::vector<Comparable> sortedVector, Comparable value) {
        auto iterator = std::lower_bound(sortedVector.begin(), sortedVector.end(), value);
        if (iterator == sortedVector.end() || *iterator != value) {
            return -1;
        } else {
            std::size_t index = std::distance(sortedVector.begin(), iterator);
            return index;
        }
    }
};


#endif //GREEDYALGORITHM_SEARCHALGORITHMS_H
