//
// Created by rbernard on 22/01/24.
//

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include <fstream>
#include <charconv>
#include <random>
#include <algorithm>
#include "Graph.h"
#include "Node.h"
#include "../../services/CSV/CSVRange.h"

Graph::Graph(const std::string& nodesFilePath, const std::string& edgesFilePath, const std::string& ptLinesFilePath) {

    //Nodes instantiation
    double x;
    double y;
    Status status;
    std::ifstream nodesFile(nodesFilePath);
    std::cout << "Nodes instantiation" << std::endl;
    for(auto& row: CSVRange(nodesFile))
    {
        //If no header, do the thing
        if(!static_cast<std::string>(row[0]).starts_with('#'))
        {
            std::from_chars(row[1].data(), row[1].data() + row[1].size(), x);
            std::from_chars(row[2].data(), row[2].data() + row[2].size(), y);
            this->nodesVector.emplace_back(Status::work, x, y);
            DEBUG_MSG("Created new node " << x << " " << y);
        }
    }

    //Edges instantiation
    int edge_start;
    int edge_end;
    double length;
    std::ifstream edgesFile(edgesFilePath);
    std::cout << "Edges instantiation" << std::endl;
    for(auto& row: CSVRange(edgesFile))
    {
        //If no header, do the thing
        if(!static_cast<std::string>(row[0]).starts_with('#'))
        {
            std::from_chars(row[0].data(), row[0].data() + row[0].size(), edge_start);
            std::from_chars(row[1].data(), row[1].data() + row[1].size(), edge_end);
            std::from_chars(row[2].data(), row[2].data() + row[2].size(), length);
            this->edgesVector.emplace_back(edge_start, edge_end, length);
            DEBUG_MSG("Created new edge between " << edge_start << " and " << edge_end << " of length " << length);
        }
    }

    //PT Lines instantiation
    int startTime = 300; //day startTime in min (5am)
    int endTime = 1440; //day endTime (12am)
    int frequency;
    int currentNode;
    std::mt19937 rng;
    rng.seed(123456789);
    std::uniform_int_distribution<uint32_t> uint_dist10(1,10);
    std::uniform_int_distribution<uint32_t> uint_dist60(1,60);
    std::ifstream ptLinesFile(ptLinesFilePath);
    std::cout << "Lines instantiation" << std::endl;
    for(auto& row: CSVRange(ptLinesFile))
    {
        //If no header, do the thing
        if(!static_cast<std::string>(row[0]).starts_with('#'))
        {
            //add nodes for the line
            Line newLine = Line();
            for(int i = 2; i < row.size(); ++i)
            {
                std::from_chars(row[i].data(), row[i].data() + row[i].size(), currentNode);
                newLine.addNode(currentNode);
            }

            //Create timetable for starting point
            std::vector<int> timeTable;
            std::from_chars(row[0].data(), row[0].data() + row[0].size(), frequency);
            std::from_chars(row[1].data(), row[1].data() + row[1].size(), startTime);
            int currentTime = startTime + uint_dist60(rng); //random startTime time with 60min max offset
            while(currentTime + frequency < endTime)
            {
                timeTable.push_back(currentTime + frequency);
                currentTime += frequency;
            }
            newLine.addTimetable(timeTable);

            //Create subsequent timetables according to preceding timetable and travel time
            for(int i = 1; i < newLine.getNodes().size(); ++i)
            {
                int travelTime = uint_dist10(rng); //FIXME travel time is randomized for now, we should get edge length if it exists I guess
                std::vector<int> precedingTimeTable = newLine.getTimetable(i - 1);
                std::vector<int> newTimetable;
                for(auto it = precedingTimeTable.begin(); it != precedingTimeTable.end(); ++it)
                {
                    newTimetable.emplace_back(*it.base() + travelTime);
                }
                newLine.addTimetable(newTimetable);
                newTimetable.clear();
            }

            DEBUG_MSG("Created new line with nodes");
        }
    }
    std::cout << "test is done" << std::endl;
}

Graph::Graph(const std::string& datFilePath) {
    std::ifstream infile(datFilePath);
    DATRow currentRow = DATRow(',');
    std::string currentLine;

    //-- Read params
    infile >> currentRow;
    std::cout << currentRow[0] << std::endl;
    // Seeded random number generator
    infile >> currentRow;
    unsigned long rngSeed;
    std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].length(), rngSeed);
    auto rng = std::mt19937(rngSeed);

    //-- End of params

    //-- Read nodes
    infile >> currentRow; // Read and print comment line for format
    std::cout << currentRow.toString() << std::endl;
    std::string status; double x,y;
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        status = currentRow[0]; //TODO:parse status enum value from string
        std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].length(), x);
        std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].length(), y);

        this->nodesVector.emplace_back(Status::work, x, y);
        DEBUG_MSG("Created new node " << x << " " << y << " with status = " << status);
    }
    //-- End of nodes

    //-- Read Edges
    std::cout << currentRow.toString() << std::endl;
    int og_idx, dest_idx; double length;
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].length(), og_idx);
        std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].length(), dest_idx);
        std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].length(), length);

        this->edgesVector.emplace_back(og_idx, dest_idx, length);
        DEBUG_MSG("Created new edge between " << og_idx << " and " << dest_idx << " of length " << length);
    }
    //-- End of edges

    //-- Read Public transit line
    std::cout << currentRow.toString() << std::endl;
    int startTime, endTime, frequency, currentNodeIdx;
    std::uniform_int_distribution<uint32_t> uint_dist10(1,10);
    std::uniform_int_distribution<uint32_t> uint_dist60(1,60);
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        //add nodes for the line
        Line newLine = Line();

        //Create a base timetable. It'll be used as a basis to generate subsequent nodes' timetables
        std::vector<int> timeTable;
        std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].size(), frequency);
        std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].size(), startTime);
        std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[3].size(), endTime);
        int currentTime = startTime + uint_dist60(rng); //random startTime time with 60min max offset
        while(currentTime + frequency < endTime)
        {
            timeTable.push_back(currentTime + frequency);
            currentTime += frequency;
        }
        newLine.addTimetable(timeTable);

        for(int i = 3; i < currentRow.size(); ++i)
        {
            std::from_chars(currentRow[i].data(), currentRow[i].data() + currentRow[i].size(), currentNodeIdx);
            newLine.addNode(currentNodeIdx);
        }

        //Create subsequent timetables according to preceding timetable and travel time
        for(int i = 1; i < newLine.getNodes().size(); ++i)
        {
            int travelTime = uint_dist10(rng); //FIXME travel time is randomized for now, we should get edge length if it exists I guess
            std::vector<int> precedingTimeTable = newLine.getTimetable(i - 1);
            std::vector<int> newTimetable;
            for(auto it = precedingTimeTable.begin(); it != precedingTimeTable.end(); ++it)
            {
                newTimetable.emplace_back(*it.base() + travelTime);
            }
            newLine.addTimetable(newTimetable);
            newTimetable.clear();
        }

        this->addLine(newLine);

        DEBUG_MSG("Created new line with nodes");
    }
}

namespace fs = std::filesystem;
void Graph::exportGraphToFiles(fs::path exportFolderPath) {
    fs::create_directories(exportFolderPath);

    //Nodes
    std::ofstream outfileNodes(exportFolderPath.string() + "nodes.txt", std::ofstream::out | std::ofstream::trunc); //open and clear file if it already existed
    for(auto& node : this->nodesVector)
    {
        outfileNodes << node.getCoordinates().x << " " << node.getCoordinates().y << std::endl;
    }
    outfileNodes.close();

    //Edges
    std::ofstream outfileEdges(exportFolderPath.string() + "edges.txt", std::ofstream::out |  std::ofstream::trunc); //open and clear file if it already existed
    for(auto& edge : this->edgesVector)
    {
        outfileEdges << edge.getNodeStart() << " " << edge.getNodeEnd() << " " << edge.getLength() << std::endl;
    }
    outfileEdges.close();

    //Transit lines
    std::ofstream outfilePT(exportFolderPath.string() + "ptlines.txt", std::ofstream::out |  std::ofstream::trunc); //open and clear file if it already existed
    for(auto& ptline : this->transitLines)
    {
        //Print nodes in order on one line
        std::ostringstream oss;
        if (!ptline.getNodes().empty())
        {
            // Convert all but the last element to avoid a trailing ","
            std::copy(ptline.getNodes().begin(), ptline.getNodes().end()-1,
                      std::ostream_iterator<int>(oss, " "));

            // Now add the last element with no delimiter
            oss << ptline.getNodes().back();
        }
        outfilePT << oss.str() << std::endl;

        //Reuse string stream to print schedules line by line
        for(auto& schedule : ptline.getTimetables())
        {
            if (!schedule.empty())
            {
                // Convert all but the last element to avoid a trailing ","
                std::copy(schedule.begin(), schedule.end()-1,
                          std::ostream_iterator<int>(oss, " "));

                // Now add the last element with no delimiter
                oss << schedule.back();
            }
            outfilePT << oss.str() << std::endl;
            oss.clear();
        }
    }
    outfilePT.close();
}
