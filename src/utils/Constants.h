//
// Created by romain on 24/06/24.
//

#ifndef GREEDYALGORITHM_CONSTANTS_H
#define GREEDYALGORITHM_CONSTANTS_H


class Constants {
public:
    static constexpr uint VEHICLE_CAPACITY = 4;
    static constexpr uint MAX_TRANSIT_CONNECTIONS = 2;
    /**
     * The max amount of closest stations we preprocess for a given node (namely request origin nodes)
     */
    static constexpr size_t MAX_CLOSEST_STATIONS_CANDIDATES = 30;
    /**
     * The maximum amount of transit entry stations checked during our heuristic algorithm
     */
    static constexpr size_t MAX_TRANSIT_ENTRY_CANDIDATES = 10;
    /**
     * The maximum amount of transit exit stations checked during our heuristic algorithm
     */
    static constexpr size_t MAX_TRANSIT_EXIT_CANDIDATES = 10;
    static constexpr double DEFAULT_DELTA_RATIO = 1.5;
/**
     * A ratio between 1 and deltaRatio that'll allow us to compute each
     * request's transit travel time ratio with regards to its delta ratio
     */
    static constexpr double BASE_TRANSIT_TRAVEL_TIME_RATIO = 1.4;
};


#endif //GREEDYALGORITHM_CONSTANTS_H
