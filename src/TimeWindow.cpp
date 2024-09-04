//
// Created by romain on 22/03/24.
//

#include "TimeWindow.h"


TimeWindow TimeWindow::operator+(uint rhs) const {
    return TimeWindow(this->min + rhs, this->max + rhs);
}

TimeWindow& TimeWindow::operator+=(uint rhs) {
    this->min += rhs;
    this->max += rhs;
    return *this;
}

TimeWindow TimeWindow::operator-(uint rhs) const {
    return TimeWindow(this->min - rhs, this->max - rhs);
}

TimeWindow& TimeWindow::operator-=(uint rhs) {
    this->min -= rhs;
    this->max -= rhs;
    return *this;
}

TimeWindow TimeWindow::operator+(const TimeWindow& rhs) const {
    return TimeWindow(this->min + rhs.min, this->max + rhs.max);
}

TimeWindow& TimeWindow::operator+=(const TimeWindow& rhs) {
    this->min += rhs.min;
    this->max += rhs.max;
    return *this;
}

TimeWindow TimeWindow::operator-(const TimeWindow& rhs) const {
    return TimeWindow(this->min - rhs.min, this->max - rhs.max);
}

TimeWindow& TimeWindow::operator-=(const TimeWindow& rhs) {
    this->min -= rhs.min;
    this->max -= rhs.max;
    return *this;
}

bool TimeWindow::isInTimeWindow(uint instant) const {
    return this->min <= instant && this->max >= instant;
}

bool TimeWindow::isInTimeWindow(const TimeWindow &instant) const {
    return this->isInTimeWindow(instant.min) && this->isInTimeWindow(instant.max);
}

std::string TimeWindow::to_string() const {
    return "[" + std::to_string(min) + "," + std::to_string(max) + "]";
}
