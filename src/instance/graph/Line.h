//
// Created by rbernard on 29/01/2024.
//

#ifndef GREEDYALGORITHM_LINE_H
#define GREEDYALGORITHM_LINE_H


#include <utility>
#include <vector>
#include <string>
#include "../../utils/SearchAlgorithms.h"

class Line {
private:
    std::string _lineID;
    std::vector<int> _nodes; //index according to Graph::_nodes, sorted according to line order (start -> terminus)
    std::vector<std::vector<int>> _timetables; //list of list of timestamps for each node start order (size of _timetables must remain constant throughout the whole vector
public:
    Line() = default;

    [[nodiscard]] const std::string &getLineId() const;
    void setLineId(const std::string &lineId);
    [[nodiscard]] std::vector<int> getNodes() const { return _nodes;};
    [[nodiscard]] int getNode(int index) const { return _nodes.at(index);};
    void addNode(const int node){this->_nodes.emplace_back(node);};
    [[nodiscard]] bool isEmpty() const{return this->_nodes.empty() || _timetables.empty();}

    [[nodiscard]] std::vector<int> getTimetable(int pos) const{ return _timetables.at(pos);};
    [[nodiscard]] std::vector<std::vector<int>> getTimetables() const{ return _timetables;};
    void addTimetable(const std::vector<int>& timetable) { _timetables.push_back(timetable);};
    void setTimetable(int pos, const std::vector<int>& timetable) { _timetables.insert(_timetables.begin() + pos, timetable);};

    /**
     * Searches for the next scheduled passage at a given station after a given instant O(log n)
     * @param stationIdx The station number for which we want to search the schedule
     * @param instant The instant of arrival at the station, hence we look for the value itself or the first greater instant
     * @return a vector index corresponding to the next valid passage at the given station and after or at the given instant
     */
    [[nodiscard]] size_t findNextScheduledPassage(int stationIdx, int instant) const {
        return SearchAlgorithms<int>::findNextSortedValue(_timetables.at(stationIdx), instant);
    }
    /**
     * Returns the instant for the given station at a given schedule position, O(1)
     * @param stationIdx The station number for which we want to search the schedule
     * @param scheduleIdx The expected index of our passage
     * @return The instant associated with the given station at the given schedule position
     */
    [[nodiscard]] int getInstant(int stationIdx, int scheduleIdx) const { return _timetables.at(stationIdx).at(scheduleIdx); }

    [[nodiscard]] size_t size() const {return _nodes.size();}
    [[nodiscard]] size_t scheduleSize() const {return _timetables.empty() ? 0 : _timetables.at(0).size();}

    [[nodiscard]] bool check() const;
    [[nodiscard]] bool checkSchedules() const;

    bool operator==(const Line &rhs) const {
        return _lineID == rhs.getLineId();
    }

    bool operator!=(const Line &rhs) const {
        return _lineID != rhs.getLineId();
    }

    Line& operator=(const Line &rhs) {
        _lineID = rhs.getLineId();
        _nodes = rhs.getNodes();
        _timetables = rhs.getTimetables();
        return *this;
    }

};

#include "Node.h"

#endif //GREEDYALGORITHM_LINE_H
