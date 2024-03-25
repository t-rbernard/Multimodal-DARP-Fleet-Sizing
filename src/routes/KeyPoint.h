//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_KEYPOINT_H
#define GREEDYALGORITHM_KEYPOINT_H


#include "../TimeWindow.h"

class KeyPoint {
protected:
    int _nodeIndex{-1};
    int _arrivalInstant{-1};
    int _departureInstant{-1};

    //Constraint propagation variables
    TimeWindow _arrivalTimeWindow{};
    TimeWindow _departureTimeWindow{};

public:
    KeyPoint();
    KeyPoint(int nodeIndex, int arrivalInstant, int departureInstant);

    [[nodiscard]] int getNodeIndex() const;
    void setNodeIndex(int nodeIndex);

    [[nodiscard]] int getArrivalInstant() const;
    void setArrivalInstant(int arrivalInstant);

    [[nodiscard]] int getDepartureInstant() const;
    void setDepartureInstant(int departureInstant);

    [[nodiscard]] const TimeWindow &getArrivalTimeWindow() const;

    void setArrivalTimeWindow(const TimeWindow &arrivalTimeWindow);

    [[nodiscard]] const TimeWindow &getDepartureTimeWindow() const;

    void setDepartureTimeWindow(const TimeWindow &departureTimeWindow);


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

    [[nodiscard]] virtual bool check() const;

    ~KeyPoint() = default;
};


#endif //GREEDYALGORITHM_KEYPOINT_H
