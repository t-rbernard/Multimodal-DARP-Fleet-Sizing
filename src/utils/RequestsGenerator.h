//
// Created by romain on 11/06/24.
//

#ifndef GREEDYALGORITHM_REQUESTSGENERATOR_H
#define GREEDYALGORITHM_REQUESTSGENERATOR_H


#include <vector>
#include "../instance/requests/Request.h"

class RequestsGenerator {
public:

    /**
     * Parametrized request generation method.
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param requestAmount How many requests to generate
     * @param deltaRatio ratio by which the direct path duration for a request is multiplied to get the max routing time
     * @param deltaMinDuration minimum delta value added to the direct path duration to get a max delta time
     * @param timeWindowWidth size of the time window available on arrival
     * @param periodStartTime start time for the period during which requests can be served
     * @param periodEndTime end time for the period during which requests can be served
     * @param rngSeed a seed to ensure reproducibility
     * @return a vector of randomly generated requests according to the given parameters
     */
    [[nodiscard]] static std::vector<Request> generateRequests(const Graph& graph, size_t requestAmount,
                                          double deltaRatio, uint deltaMinDuration, uint timeWindowWidth,
                                          uint periodStartTime, uint periodEndTime, ulong rngSeed);

    /**
     * Default parameter request generation.
     * Parameters :
     *  Delta ratio = 1.25 (users will accept at most a trip 25% longer than the shortest path)
     *  Time Window width = 30 (users will have a 30 minute range of acceptable arrival dates)
     *  Delta ratio = 1.25
     *  Delta min leeway = 10 minutes of minimum possible detour time to calculate delta value
     *  periodStartTime = 360 (requests start after 6am)
     *  periodEndTime = 660 (requests end before 11am)
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param requestAmount How many requests to generate
     * @param rngSeed a seed to ensure reproducibility
     * @return a vector of randomly generated requests according to the given parameters
     */
    [[nodiscard]] static std::vector<Request> generateRequests(const Graph& graph, size_t requestAmount, ulong rngSeed);

    /**
     * Default parameter request generation with file export.
     * Parameters :
     *  Delta ratio = 1.25 (users will accept at most a trip 25% longer than the shortest path)
     *  Time Window width = 30 (users will have a 30 minute range of acceptable arrival dates)
     *  Delta ratio = 1.25
     *  Delta min leeway = 10 minutes of minimum possible detour time to calculate delta value
     *  periodStartTime = 360 (requests start after 6am)
     *  periodEndTime = 660 (requests end before 11am)
     * @param exportFolderPath folder path to which the requests will be exported in a "requests.dat" file, a requestsInfo.txt will be exported with seed/parameter info (will overwrite any existing content)
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param requestAmount How many requests to generate
     * @param rngSeed a seed to ensure reproducibility
     * @return True iff all requests were generated and exported properly
     */
    [[nodiscard]] static bool generateAndExportRequests(const std::string exportFolderPath, const Graph& graph, size_t requestAmount, ulong rngSeed);

    /**
     * Parametrized request generation method with file export.
     * @param exportFolderPath folder path to which the requests will be exported in a "requests.dat" file, a requestsInfo.txt will be exported with seed/parameter info (will overwrite any existing content)
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param requestAmount How many requests to generate
     * @param deltaRatio ratio by which the direct path duration for a request is multiplied to get the max routing time
     * @param deltaMinDuration minimum delta value added to the direct path duration to get a max delta time
     * @param timeWindowWidth size of the time window available on arrival
     * @param periodStartTime start time for the period during which requests can be served
     * @param periodEndTime end time for the period during which requests can be served
     * @param rngSeed a seed to ensure reproducibility
     * @return True iff all requests were generated and exported properly
     */
    [[nodiscard]] static bool generateAndExportRequests(const std::string exportFolderPath, const Graph& graph, size_t requestAmount,
                                                        double deltaRatio, uint deltaMinDuration, uint timeWindowWidth,
                                                        uint periodStartTime, uint periodEndTime, ulong rngSeed);

};


#endif //GREEDYALGORITHM_REQUESTSGENERATOR_H
