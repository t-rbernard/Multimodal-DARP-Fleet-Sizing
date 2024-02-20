//
// Created by rbernard on 22/01/24.
//

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include <sstream>
#include <charconv>
#include <random>
#include <algorithm>
#include "Node.h"
#include "../../services/CSV/CSVRange.h"
#include "Graph.h"

std::vector<Line> Graph::addLine(Line& line) {
    //Add line stops to nodes
    for(int i = 0; i < line.getNodes().size(); ++i)
    {
        nodesVector.at(i).addBusLine(line, i);
    }
    //Add transit line to transit lines vector
    transitLines.push_back(line);
    return transitLines;
}

Graph::Graph(const std::string& nodesFilePath, const std::string& edgesFilePath, const std::string& ptLinesFilePath) {

    //Nodes instantiation
    double x;
    double y;
    Status status;
    std::ifstream nodesFile(nodesFilePath);
    std::cout << "Nodes instantiation" << std::endl;
    for(auto& row: CSVRange(nodesFile))
    {
        parseNodeRow(row);
    }

    //Edges instantiation
    std::ifstream edgesFile(edgesFilePath);
    std::cout << "Edges instantiation" << std::endl;
    for(auto& row: CSVRange(edgesFile))
    {
        this->parseEdgeRow(row);
    }

    //PT Lines instantiation
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
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        this->parseNodeRow(currentRow);
    }
    //-- End of nodes

    //-- Read Edges
    std::cout << currentRow.toString() << std::endl;
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        this->parseEdgeRow(currentRow);
    }
    //-- End of edges

    //-- Read Public transit line
    std::cout << currentRow.toString() << std::endl;
    std::uniform_int_distribution<uint32_t> uint_dist10(1,10);
    std::uniform_int_distribution<uint32_t> uint_dist60(1,60);
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        this->parseLineRandomizedSchedule(currentRow, rng, uint_dist10, uint_dist60);
    }
}

namespace fs = std::filesystem;
void Graph::exportGraphToFiles(fs::path exportFolderPath) {
    fs::create_directories(exportFolderPath);

    //Nodes
    std::ofstream outfileNodes(exportFolderPath.string() + "nodes.txt", std::ofstream::out | std::ofstream::trunc); //open and clear file if it already existed
    for(auto& node : this->nodesVector)
    {
        outfileNodes << node.getX() << " " << node.getY() << std::endl;
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
        std::ostringstream ossNodes;
        std::vector<int> lineNodesVector = ptline.getNodes();
        if (!lineNodesVector.empty())
        {
            // Convert all but the last element to avoid a trailing ","
            std::copy(lineNodesVector.begin(), lineNodesVector.end()-1,
                      std::ostream_iterator<int>(ossNodes, " "));

            // Now add the last element with no delimiter
            ossNodes << lineNodesVector.back();
        }
        std::cout << ossNodes.view() << std::endl;
        outfilePT << ossNodes.str() << std::endl;
        ossNodes.clear();

        //Reuse string stream to print schedules line by line
        for(auto& schedule : ptline.getTimetables())
        {
            std::ostringstream ossSchedule;
            if (!schedule.empty())
            {
                // Convert all but the last element to avoid a trailing ","
                std::copy(schedule.begin(), schedule.end()-1,
                          std::ostream_iterator<int>(ossSchedule, " "));

                // Now add the last element with no delimiter
                ossSchedule << schedule.back();
            }
            std::cout << ossSchedule.view() << std::endl;
            outfilePT << ossSchedule.str() << std::endl;
            ossSchedule.clear();
        }
        outfilePT << "#PT line end" <<std::endl;
    }
    outfilePT.close();
    std::cout << "results of graph validations : " << this->check() << std::endl;
}

bool Graph::check() {
    bool checkResult = true;
    for(auto& transitLine : this->transitLines)
    {
        checkResult &= transitLine.check();
    }
    checkResult &= checkLineToNodeLinks();
    return checkResult;
}

bool Graph::checkLineToNodeLinks() {
    int nodeIndexFromLine;
    Node& nodeFromGraph(nodesVector.at(0)); //Forced to init here

    bool checkResult = true;
    for(auto& node : nodesVector)
    {
        for(auto& lineStop : node.getPTLinesSet())
        {
            nodeIndexFromLine = lineStop.getLineRef().getNode(lineStop.getStopIndex());
            nodeFromGraph = this->nodesVector.at(nodeIndexFromLine);
            checkResult &= nodeFromGraph == node;
        }
    }
    return checkResult;
}

void Graph::parseNodeRow(const DATRow& row)
{
    //Skip line if it starts with a # (comment sign)
    if(!static_cast<std::string>(row[0]).starts_with('#')) {
        Status status;
        double x, y;

        status = Node::statusFromString(std::string(row[0]));
        std::from_chars(row[1].data(), row[1].data() + row[1].length(), x);
        std::from_chars(row[2].data(), row[2].data() + row[2].length(), y);

        this->nodesVector.emplace_back(status, x, y);
        DEBUG_MSG("Created new node " << x << " " << y << " with status = " << status);
    }
}

void Graph::parseEdgeRow(const DATRow& row)
{
    int edge_start;
    int edge_end;
    double length;
    //Skip line if it starts with a # (comment sign)
    if(!static_cast<std::string>(row[0]).starts_with('#'))
    {
        std::from_chars(row[0].data(), row[0].data() + row[0].size(), edge_start);
        std::from_chars(row[1].data(), row[1].data() + row[1].size(), edge_end);
        std::from_chars(row[2].data(), row[2].data() + row[2].size(), length);
        this->createAndAddEdge(edge_start, edge_end, length);
        DEBUG_MSG("Created new edge between " << edge_start << " and " << edge_end << " of length " << length);
    }
}

void Graph::parseLineRandomizedSchedule(const DATRow& row, std::mt19937 rng,
                                        std::uniform_int_distribution<uint32_t> travelTimeDistribution,
                                        std::uniform_int_distribution<uint32_t> startTimeDistribution)
{
    int startTime, endTime, frequency, currentNodeIdx;
    //add nodes for the line
    Line newLine = Line();

    //Create a base timetable. It'll be used as a basis to generate subsequent nodes' timetables
    std::vector<int> timeTable;
    std::from_chars(row[0].data(), row[0].data() + row[0].size(), frequency);
    std::from_chars(row[1].data(), row[1].data() + row[1].size(), startTime);
    std::from_chars(row[2].data(), row[2].data() + row[2].size(), endTime);
    int currentTime = startTime + startTimeDistribution(rng); //random startTime time with 60min max offset
    while(currentTime + frequency < endTime)
    {
        timeTable.push_back(currentTime + frequency);
        currentTime += frequency;
    }
    newLine.addTimetable(timeTable);

    for(int i = 3; i < row.size(); ++i)
    {
        std::from_chars(row[i].data(), row[i].data() + row[i].size(), currentNodeIdx);
        newLine.addNode(currentNodeIdx);
    }

    //Create subsequent timetables according to preceding timetable and travel time
    for(int i = 1; i < newLine.getNodes().size(); ++i)
    {
        int travelTime = travelTimeDistribution(rng); //FIXME travel time is randomized for now, we should get edge length if it exists I guess
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

void Graph::createAndAddEdge(int edgeStartNodeIndex, int edgeEndNodeIndex, double length) {
    edgesVector.emplace_back(edgeStartNodeIndex, edgeEndNodeIndex, length);

    Node entryNode = nodesVector.at(edgeStartNodeIndex);
    entryNode.getOutgoingEdges().emplace_back(edgesVector.size() - 1);

    Node exitNode = nodesVector.at(edgeEndNodeIndex);
    exitNode.getIncomingEdges().emplace_back(edgesVector.size() - 1);
}
