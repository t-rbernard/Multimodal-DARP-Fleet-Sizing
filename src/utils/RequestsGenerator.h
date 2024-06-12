//
// Created by romain on 11/06/24.
//

#ifndef GREEDYALGORITHM_REQUESTSGENERATOR_H
#define GREEDYALGORITHM_REQUESTSGENERATOR_H


#include <vector>
#include <filesystem>
#include "../instance/requests/Request.h"
#include "RequestGenerationParameters.h"

namespace fs = std::filesystem;

class RequestsGenerator {
public:

    /**
     * Parametrized request generation method.
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param generationParameters the list of generation parameters to use for this request generation (see RequestGeberationParameters constructors)
     * @return a vector of randomly generated requests according to the given parameters
     */
    [[nodiscard]] static std::vector<Request> generateRequests(const Graph& graph, RequestGenerationParameters generationParameters);

    /**
     * Parametrized request generation method with file export.
     * @param exportFolderPath folder path to which the requests will be exported in a "requests.dat" file, a requestsInfo.txt will be exported with seed/parameter info (will overwrite any existing content)
     * @param graph the graph on which requests are generated (required for node lists and shortest paths between nodes)
     * @param generationParameters the list of generation parameters to use for this request generation (see RequestGeberationParameters constructors)
     * @return a vector of randomly generated requests according to the given parameters
     */
    [[nodiscard]] static std::vector<Request> generateAndExportRequests(const fs::path &exportFolderPath, const Graph& graph, const RequestGenerationParameters &generationParameters);

};


#endif //GREEDYALGORITHM_REQUESTSGENERATOR_H
