//
// Created by rbernard on 22/01/24.
//

#include <charconv>
#include "Request.h"
#include "../../services/DatFile/DATRow.h"

Request::Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const uint deltaTime, const uint weight) : _originNodeIndex(departureNodeIndex),
                                                            _destinationNodeIndex(arrivalNodeIndex), _arrivalTW(arrivalTw),
                                                            _deltaTime(deltaTime), _weight(weight) {
    _departureTW = _arrivalTW - deltaTime;

    if(_departureTW.min > _departureTW.max || _arrivalTW.min > _arrivalTW.max)
        throw TimeWindow::invalid_time_window_exception();
}

Request::Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const uint deltaTime, const uint weight, const Graph& graph) :
        _originNodeIndex(departureNodeIndex), _destinationNodeIndex(arrivalNodeIndex),
        _arrivalTW(arrivalTw), _deltaTime(deltaTime), _weight(weight) {
    _departureTW.min = _arrivalTW.min - deltaTime;
    _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(departureNodeIndex, arrivalNodeIndex);

    if(_departureTW.min > _departureTW.max || _arrivalTW.min > _arrivalTW.max)
        throw TimeWindow::invalid_time_window_exception();
}

Request::Request(const DATRow& currentRow, const Graph& graph) {
    std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].size(), _originNodeIndex);
    std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].size(), _destinationNodeIndex);

    uint twMin, twMax;
    bool setDepartureTW{false};
    bool setArrivalTW{false};
    if(currentRow.size() == 8) { //More flexible 8 arguments initialization where we can set either origin or destination TW or both
        if (!currentRow[2].empty() && !currentRow[3].empty()) { //Departure TW
            std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].size(), twMin);
            std::from_chars(currentRow[3].data(), currentRow[3].data() + currentRow[3].size(), twMax);
            _departureTW = TimeWindow(twMin, twMax);
            setDepartureTW = true;
        }
        if (!currentRow[4].empty() && !currentRow[5].empty()) { //Arrival TW
            std::from_chars(currentRow[4].data(), currentRow[4].data() + currentRow[4].size(), twMin);
            std::from_chars(currentRow[5].data(), currentRow[5].data() + currentRow[5].size(), twMax);
            _arrivalTW = TimeWindow(twMin, twMax);
            setArrivalTW = true;
        }

        std::from_chars(currentRow[6].data(), currentRow[6].data() + currentRow[6].size(), _deltaTime);
        std::from_chars(currentRow[7].data(), currentRow[7].data() + currentRow[7].size(), _weight);
    } else if (currentRow.size() == 6) { //Original 6 inputs request creation, setting a destination time window (needed for compatibility)
        std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].size(), twMin);
        std::from_chars(currentRow[3].data(), currentRow[3].data() + currentRow[3].size(), twMax);
        _arrivalTW = TimeWindow(twMin, twMax);
        setArrivalTW = true;

        std::from_chars(currentRow[4].data(), currentRow[4].data() + currentRow[4].size(), _deltaTime);
        std::from_chars(currentRow[5].data(), currentRow[5].data() + currentRow[5].size(), _weight);
    }


    if(!setDepartureTW) {
        _departureTW.min = _arrivalTW.min - _deltaTime;
        _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex);
    }
    if(!setArrivalTW) {
        _arrivalTW.min = _departureTW.min + graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex);
        _arrivalTW.max = _departureTW.max + _deltaTime;
    }
    if(_departureTW.min > _departureTW.max || _arrivalTW.min > _arrivalTW.max)
        throw TimeWindow::invalid_time_window_exception();
}

Request::Request(const DATRow& currentRow, double deltaRatio, const Graph& graph) : Request(currentRow, graph){
    _deltaTime = floor(_deltaTime * deltaRatio);
}

std::vector<Request> Request::getRequestsFromFile(const std::string& datFilePath, const Graph& graph) {
    std::vector<Request> requests;

    std::ifstream infile(datFilePath);
    assertm(!infile.fail(), "Failed to open the given file");
    DATRow currentRow = DATRow(',');

    //-- Read params
    infile >> currentRow;
    std::cout << currentRow[0] << std::endl;
    // Delta ratio
    infile >> currentRow;
    double deltaRatio;
    std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].length(), deltaRatio);
    //-- End of params

    //-- Read requests
    infile >> currentRow; // Read and print comment line for format
    std::cout << currentRow.toString() << std::endl;
    while(infile >> currentRow && !currentRow[0].starts_with('#')) {
        requests.emplace_back(currentRow, deltaRatio, graph);
    }

    return requests;
}

size_t Request::getOriginNodeIndex() const {
    return _originNodeIndex;
}

size_t Request::getDestinationNodeIndex() const {
    return _destinationNodeIndex;
}

const TimeWindow &Request::getArrivalTw() const {
    return _arrivalTW;
}

uint Request::getDeltaTime() const {
    return _deltaTime;
}

uint Request::getWeight() const {
    return _weight;
}

const RequestRoute &Request::getCurrentRoute() const {
    return _currentRoute;
}

const TimeWindow &Request::getDepartureTw() const {
    return _departureTW;
}

uint Request::getMinDepartureTw() const {
    return _departureTW.min;
}


uint Request::getMaxDepartureTw() const {
    return _departureTW.max;
}

uint Request::getMinArrivalTw() const {
    return _arrivalTW.min;
}


uint Request::getMaxArrivalTw() const {
    return _arrivalTW.max;
}

//-----------------------------
//---------- Setters ----------
//-----------------------------
void Request::setCurrentRoute(const RequestRoute &currentRoute) {
    _currentRoute = currentRoute;
}

void Request::setDepartureTw(const TimeWindow &departureTw) {
    _departureTW = departureTw;
}

//-----------------------------
//--------- Route API ---------
//-----------------------------

int Request::getNodeIndex(int routeIndex) const {
    return _currentRoute.getNodeIndex(routeIndex);
}

void Request::setNodeIndex(int routeIndex, int nodeIndex) {
    _currentRoute.setNodeIndex(routeIndex, nodeIndex);
}

SAEVehicle const *Request::getSAEV(int routeIndex) const {
    return _currentRoute.getSAEV(routeIndex);
}

void Request::setSAEV(int routeIndex, SAEVehicle *saev) {
    _currentRoute.setSAEV(routeIndex, saev);
}

LineStop const *Request::getLineStop(int routeIndex) const {
    return _currentRoute.getLineStop(routeIndex);
}

void Request::setLineStop(int routeIndex, LineStop *lineStop) {
    _currentRoute.setLineStop(routeIndex, lineStop);
}

void Request::resetKeyPoint(int routeIndex) {
    _currentRoute.resetKeyPoint(routeIndex);
}

std::string Request::to_string() const {
    std::string res = "O=" + std::to_string(_originNodeIndex) + "," + "D=" + std::to_string(_destinationNodeIndex) //FIXME:replace with std::format whenever possible
                      + ", " + _departureTW.to_string() + ", " + _arrivalTW.to_string()
                      + ", w=" + std::to_string(_weight);
    return res;
}

std::string Request::to_string_export() const {
    std::string res = std::to_string(_originNodeIndex) + "," + std::to_string(_destinationNodeIndex) //FIXME:replace with std::format whenever possible
            + "," + std::to_string(_arrivalTW.min) + "," + std::to_string(_arrivalTW.max)
            + "," + std::to_string(_deltaTime) + "," + std::to_string(_weight);
    return res;
}

Request::Request(const Graph &graph, const Request &baseRequest, const TransitAccess &transitEntry) {
    _originNodeIndex = baseRequest.getOriginNodeIndex();
    _destinationNodeIndex = transitEntry.getAccessNodeIdx();

    _departureTW = baseRequest.getDepartureTw();

    _arrivalTW.min = baseRequest.getDepartureTw().min + graph.getShortestSAEVPath(_originNodeIndex, transitEntry.getAccessNodeIdx());
    _arrivalTW.max = transitEntry.getAccessTimestamp();

    if(_departureTW.min > _departureTW.max || _arrivalTW.min > _arrivalTW.max)
        throw TimeWindow::invalid_time_window_exception();

    _transitTravelTimeRatio = baseRequest.getTransitTravelTimeRatio();
    _deltaTime = UINT16_MAX;
    _weight = baseRequest.getWeight();
}

Request::Request(const Graph &graph, const Request &baseRequest, const TransitAccess &transitExit,
                 const SAEVKeyPoint &originSubRequestKeyPoint) {
    _originNodeIndex = transitExit.getAccessNodeIdx();
    _destinationNodeIndex = baseRequest.getDestinationNodeIndex();

    _departureTW.min = transitExit.getAccessTimestamp();
    _departureTW.max = baseRequest.getArrivalTw().max - graph.getShortestSAEVPath(transitExit.getAccessNodeIdx(), _destinationNodeIndex);

    _arrivalTW.min = baseRequest.getArrivalTw().min;
    _arrivalTW.max = originSubRequestKeyPoint.getMinTw() + baseRequest.getDeltaTime(); //Reduce max arrival TW to a value we are 100% sure is compatible with our current min departure time

    if(_departureTW.min > _departureTW.max || _arrivalTW.min > _arrivalTW.max)
        throw TimeWindow::invalid_time_window_exception();

    _transitTravelTimeRatio = baseRequest.getTransitTravelTimeRatio();
    _deltaTime = UINT16_MAX;
    _weight = baseRequest.getWeight();
}

double Request::getTransitTravelTimeRatio() const {
    return _transitTravelTimeRatio;
}

void Request::setTransitTravelTimeRatio(double transitTravelTimeRatio) {
    _transitTravelTimeRatio = transitTravelTimeRatio;
}

double Request::computeTransitTravelTimeRatio(double deltaRatio, double travelTimeRatio) {
    double computedRatio = (1.0 + (travelTimeRatio * (1.0 - deltaRatio)));
    if(computedRatio < 1)
        return 1;
    else if(computedRatio > deltaRatio)
        return deltaRatio;
    else
        return computedRatio;
}

double Request::computeTransitTravelTimeRatio(const Graph &graph, double travelTimeRatio) const {
    double deltaRatio = (graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex))/(double)_deltaTime;
    return computeTransitTravelTimeRatio(deltaRatio, travelTimeRatio);
}
