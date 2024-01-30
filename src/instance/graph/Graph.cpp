//
// Created by rbernard on 22/01/24.
//

#include <fstream>
#include <charconv>
#include <random>
#include <algorithm>
#include "Graph.h"
#include "Node.h"
#include "../services/CSV/CSVRange.h"

Graph::Graph(std::string nodesFilePath, std::string edgesFilePath, std::string ptLinesFilePath) {

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
            std::cout << "Created new node " << x << " " << y << std::endl;
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
            std::cout << "Created new edge between " << edge_start << " and " << edge_end << " of length " << length << std::endl;
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

            std::cout << "Created new line with nodes" << std::endl;
        }
    }
    std::cout << "test is done" << std::endl;
}
