//
// Created by romain on 12/06/24.
//

#ifndef GREEDYALGORITHM_REQUESTGENERATIONPARAMETERS_H
#define GREEDYALGORITHM_REQUESTGENERATIONPARAMETERS_H


#include <cstdlib>
#include <ostream>

class RequestGenerationParameters {
private:
    size_t _requestAmount;
    double _deltaRatio;
    uint _deltaMinDuration;
    uint _timeWindowWidth;
    uint _periodStartTime;
    uint _periodEndTime;
    ulong _rngSeed;
    //finding a way to add the graph to these parameters would be good, but adding a dependency between graph and request generation parameters doesn't feel right
public:
    /**
     * Generates a request generation parameter container with default parameters (giving a seed is mandatory still !)
     * Default parameters :
     *  Delta ratio = 1.25 (users will accept at most a trip 25% longer than the shortest path)
     *  Time Window width = 30 (users will have a 30 minute range of acceptable arrival dates)
     *  Delta ratio = 1.25
     *  Delta min leeway = 10 minutes of minimum possible detour time to calculate delta value
     *  periodStartTime = 360 (requests start after 6am)
     *  periodEndTime = 660 (requests end before 11am)
     * @param requestAmount How many requests to generate
     * @param rngSeed a seed to ensure reproducibility of the generation
     */
    explicit RequestGenerationParameters(size_t requestAmount, ulong rngSeed) : _requestAmount(requestAmount), _deltaRatio(1.25), _deltaMinDuration(10), _timeWindowWidth(30),
    _periodStartTime(360), _periodEndTime(660), _rngSeed(rngSeed) {}

    /**
     * Generates a request generation parameter container with the given parameters
     * @param requestAmount How many requests to generate
     * @param deltaRatio ratio by which the direct path duration for a request is multiplied to get the max routing time
     * @param deltaMinDuration minimum delta value added to the direct path duration to get a max delta time
     * @param timeWindowWidth size of the time window available on arrival
     * @param periodStartTime start time for the period during which requests can be served
     * @param periodEndTime end time for the period during which requests can be served
     * @param rngSeed a seed to ensure reproducibility of the generation
     */
    RequestGenerationParameters(size_t requestAmount, double deltaRatio, uint deltaMinDuration, uint timeWindowWidth,
                                uint periodStartTime, uint periodEndTime, ulong rngSeed) : _requestAmount(
            requestAmount), _deltaRatio(deltaRatio), _deltaMinDuration(deltaMinDuration), _timeWindowWidth(
            timeWindowWidth), _periodStartTime(periodStartTime), _periodEndTime(periodEndTime), _rngSeed(rngSeed) {}

    size_t getRequestAmount() const {
        return _requestAmount;
    }

    void setRequestAmount(size_t requestAmount) {
        _requestAmount = requestAmount;
    }

    double getDeltaRatio() const {
        return _deltaRatio;
    }

    void setDeltaRatio(double deltaRatio) {
        _deltaRatio = deltaRatio;
    }

    uint getDeltaMinDuration() const {
        return _deltaMinDuration;
    }

    void setDeltaMinDuration(uint deltaMinDuration) {
        _deltaMinDuration = deltaMinDuration;
    }

    uint getTimeWindowWidth() const {
        return _timeWindowWidth;
    }

    void setTimeWindowWidth(uint timeWindowWidth) {
        _timeWindowWidth = timeWindowWidth;
    }

    uint getPeriodStartTime() const {
        return _periodStartTime;
    }

    void setPeriodStartTime(uint periodStartTime) {
        _periodStartTime = periodStartTime;
    }

    uint getPeriodEndTime() const {
        return _periodEndTime;
    }

    void setPeriodEndTime(uint periodEndTime) {
        _periodEndTime = periodEndTime;
    }

    ulong getRngSeed() const {
        return _rngSeed;
    }

    void setRngSeed(ulong rngSeed) {
        _rngSeed = rngSeed;
    }

    friend std::ostream &operator<<(std::ostream &os, const RequestGenerationParameters &parameters) {
        os << "_requestAmount: " << parameters._requestAmount << "\n_deltaRatio: " << parameters._deltaRatio
           << "\n_deltaMinDuration: " << parameters._deltaMinDuration << "\n_timeWindowWidth: "
           << parameters._timeWindowWidth << "\n_periodStartTime: " << parameters._periodStartTime << "\n_periodEndTime: "
           << parameters._periodEndTime << "\n_rngSeed: " << parameters._rngSeed;
        return os;
    }

    bool operator==(const RequestGenerationParameters &rhs) const = default;
};


#endif //GREEDYALGORITHM_REQUESTGENERATIONPARAMETERS_H
