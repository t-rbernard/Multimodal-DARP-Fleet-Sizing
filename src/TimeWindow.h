//
// Created by romain on 22/03/24.
//

#ifndef GREEDYALGORITHM_TIMEWINDOW_H
#define GREEDYALGORITHM_TIMEWINDOW_H

#include <cstdlib>
#include "utils/Globals.h"

/**
 * Represents [min, max] time windows with appropriate operators for TW operations (+,-,isInTimeWindow).
 * Prefer using +=/-= notations to modify in place, as + and - operators return a new TimeWindow object.
 */
struct TimeWindow {
    uint min = 0, max = 0;
    TimeWindow() = default;
    TimeWindow(uint min, uint max) : min(min), max(max) {}
    /**
     * Checks if the given instant is inside the [min,max] interval
     * @return True iff instant is in the time window, false otherwise
     */
    [[nodiscard]] bool isInTimeWindow(uint instant) const;
    /**
    * Checks if the given time window is contained inside the [min,max] interval of this
    * @return True iff instant is in the time window, false otherwise
    */
    [[nodiscard]] bool isInTimeWindow(const TimeWindow& instant) const;

    TimeWindow operator+(uint rhs) const;
    TimeWindow& operator+=(uint rhs);
    TimeWindow operator-(uint rhs) const;
    TimeWindow& operator-=(uint rhs);
    TimeWindow operator+(const TimeWindow& rhs) const;
    TimeWindow& operator+=(const TimeWindow& rhs);
    TimeWindow operator-(const TimeWindow& rhs) const;
    TimeWindow& operator-=(const TimeWindow& rhs);
    TimeWindow& operator=(const TimeWindow& rhs) = default;
};


#endif //GREEDYALGORITHM_TIMEWINDOW_H
