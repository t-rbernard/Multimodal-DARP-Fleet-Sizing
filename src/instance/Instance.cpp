//
// Created by rbernard on 22/01/24.
//

#include <iostream>
#include "Instance.h"
#include "graph/Graph.h"
#include "requests/Request.h"
#include "../services/DatFile/DATRow.h"

Instance::Instance(const std::string& graphFilePath,
                   const std::string& requestsFilePath,
                   const int vehicleCapacity): _graph(graphFilePath),_vehicleCapacity(vehicleCapacity)
{
    parseRequestsFromFile(requestsFilePath);
}

void Instance::parseRequestsFromFile(const std::string &requestsFilePath) {
    std::ifstream infile(requestsFilePath);
    DATRow currentRow = DATRow(',');
    std::string currentLine;
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        _requests.emplace_back(currentRow, _graph);
    }
}
