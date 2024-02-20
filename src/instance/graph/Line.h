//
// Created by rbernard on 29/01/2024.
//

#ifndef GREEDYALGORITHM_LINE_H
#define GREEDYALGORITHM_LINE_H


#include <utility>
#include <vector>
#include "../../utils/SearchAlgorithms.h"

class Line {
private:
    std::vector<int> _nodes; //index according to Graph::_nodes, sorted according to line order (start -> terminus)
    std::vector<std::vector<int>> _timetables; //list of list of timestamps for each node start order (size of _timetables must remain constant throughout the whole vector
    bool checkSchedules();
public:
    Line() = default;

    std::vector<int> getNodes() { return _nodes;};
    int getNode(int index) { return _nodes.at(index);};
    void addNode(const int node){this->_nodes.emplace_back(node);};

    std::vector<int> getTimetable(int pos) { return _timetables.at(pos);};
    std::vector<std::vector<int>> getTimetables() { return _timetables;};
    void addTimetable(const std::vector<int>& timetable) { _timetables.push_back(timetable);};
    void setTimetable(int pos, const std::vector<int>& timetable) { _timetables.insert(_timetables.begin() + pos, timetable);};
    //TODO : for unit tests, add Line from file creation function
    //  check(), checkSchedules(), checkNodesLink(nodesvector)
    //  Additionally, Line verifier utility functions could be good here (both debug, unit tests and instance verification => Graph.check() checks all subcomponent)

    /**
     * Searches for the next scheduled passage at a given station after a given timestep O(log n)
     * @param stationIdx The station number for which we want to search the schedule
     * @param timestep The timestep of arrival at the station, hence we look for the value itself or the first greater timestep
     * @return a vector index corresponding to the next valid passage at the given station and after or at the given timestep
     */
    size_t findNextScheduledPassage(int stationIdx, int timestep) {
        return SearchAlgorithms<int>::findNextSortedValue(_timetables.at(stationIdx), timestep);
    }
    /**
     * Returns the timestep for the given station at a given schedule position, O(1)
     * @param stationIdx The station number for which we want to search the schedule
     * @param scheduleIdx The expected index of our passage
     * @return The timestep associated with the given station at the given schedule position
     */
    int getTimestep(int stationIdx, int scheduleIdx) { return _timetables.at(stationIdx).at(scheduleIdx); }
    bool check();
};

#include "Node.h"

#endif //GREEDYALGORITHM_LINE_H
