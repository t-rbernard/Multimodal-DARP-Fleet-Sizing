//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_KEYPOINT_H
#define GREEDYALGORITHM_KEYPOINT_H


class KeyPoint {
protected:
    int _nodeIndex{};
    int _arrivalInstant{};
    int _departureInstant{};

public:
    KeyPoint(int nodeIndex, int arrivalInstant, int departureInstant);

    KeyPoint();

    [[nodiscard]] int getNodeIndex() const;
    void setNodeIndex(int nodeIndex);

    [[nodiscard]] int getArrivalInstant() const;
    void setArrivalInstant(int arrivalInstant);

    [[nodiscard]] int getDepartureInstant() const;
    void setDepartureInstant(int departureInstant);

    /**
     * Returns true if this is temporally before (or at the same time) as the given key point
     * @return true if we arrive on the given key point after (or at the same time as) when we depart from this key point
     */
    [[nodiscard]] bool isBeforeKeyPoint(const KeyPoint& kp) const;
    /**
     * Returns true if this is temporally after (or at the same time) as the given key point
     * @return true if we arrive on the given key point after (or at the same time as) when we depart from this key point
     */
     [[nodiscard]] bool isAfterKeyPoint(const KeyPoint& kp) const;

    virtual [[nodiscard]] bool check() const;
};


#endif //GREEDYALGORITHM_KEYPOINT_H
