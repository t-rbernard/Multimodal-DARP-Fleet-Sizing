//
// Created by rbernard on 29/01/2024.
//

#include "Line.h"

bool Line::check() const{
    return checkSchedules(); //Add any new check function here (if useful, add parameters to check() to only do certain checks
}

bool Line::checkSchedules() const{
    bool checkResult = true;
    checkResult &= _nodes.size() == _timetables.size(); //check we have as many schedules as nodes in our line

    int precedingTimeStep = 0;
    int expectedScheduleSize = !_timetables.empty() ? _timetables.at(0).size() : 0;
    for(auto& schedule : _timetables)
    {
        precedingTimeStep = 0; //reinit first timestep to 0
        checkResult &= schedule.size() == expectedScheduleSize; //every schedule should are the same size
        for(auto& currentTimestep : schedule) //check timestep precedence
        {
            checkResult &= currentTimestep > precedingTimeStep;
            precedingTimeStep = currentTimestep;
        }
    }
    return checkResult;
}

const std::string &Line::getLineId() const {
    return _lineID;
}

void Line::setLineId(const std::string &lineId) {
    _lineID = lineId;
}
