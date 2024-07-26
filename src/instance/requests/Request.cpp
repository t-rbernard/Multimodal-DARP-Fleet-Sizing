//
// Created by rbernard on 22/01/24.
//

#include <charconv>
#include "Request.h"
#include "../../services/DatFile/DATRow.h"

Request::Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const uint deltaTime, const uint weight) : _originNodeIndex(departureNodeIndex),
                                                            _destinationNodeIndex(arrivalNodeIndex), _arrivalTW(arrivalTw),
                                                            _deltaTime(deltaTime), _weight(weight), _currentDeltaTime(deltaTime) {
    _departureTW = _arrivalTW - deltaTime;
}

Request::Request(const size_t departureNodeIndex, const size_t arrivalNodeIndex, const TimeWindow &arrivalTw,
                 const uint deltaTime, const uint weight, const Graph& graph) :
        _originNodeIndex(departureNodeIndex), _destinationNodeIndex(arrivalNodeIndex),
        _arrivalTW(arrivalTw), _deltaTime(deltaTime), _weight(weight), _currentDeltaTime(deltaTime) {
    _departureTW.min = _arrivalTW.min - deltaTime;
    _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(departureNodeIndex, arrivalNodeIndex);
}

Request::Request(const DATRow& currentRow, const Graph& graph) {
    std::from_chars(currentRow[0].data(), currentRow[0].data() + currentRow[0].size(), _originNodeIndex);
    std::from_chars(currentRow[1].data(), currentRow[1].data() + currentRow[1].size(), _destinationNodeIndex);

    int twMin, twMax;
    bool setDepartureTW{false};
    bool setArrivalTW{false};
    if(!currentRow[2].empty() && !currentRow[3].empty()) { //Departure TW
        std::from_chars(currentRow[2].data(), currentRow[2].data() + currentRow[2].size(), twMin);
        std::from_chars(currentRow[3].data(), currentRow[3].data() + currentRow[3].size(), twMax);
        _departureTW = TimeWindow(twMin, twMax);
        setDepartureTW = true;
    }
    if(!currentRow[4].empty() && !currentRow[5].empty()) { //Arrival TW
        std::from_chars(currentRow[4].data(), currentRow[4].data() + currentRow[4].size(), twMin);
        std::from_chars(currentRow[5].data(), currentRow[5].data() + currentRow[5].size(), twMax);
        _arrivalTW = TimeWindow(twMin, twMax);
        setArrivalTW = true;
    }

    std::from_chars(currentRow[6].data(), currentRow[6].data() + currentRow[6].size(), _deltaTime);
    std::from_chars(currentRow[7].data(), currentRow[7].data() + currentRow[7].size(), _weight);

    if(!setDepartureTW) {
        _departureTW.min = _arrivalTW.min - _deltaTime;
        _departureTW.max = _arrivalTW.max - graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex);
    }
    if(!setArrivalTW) {
        _arrivalTW.min = _departureTW.min + graph.getShortestSAEVPath(_originNodeIndex, _destinationNodeIndex);
        _arrivalTW.max = _departureTW.max + _deltaTime;
    }
}

Request::Request(const DATRow& currentRow, double deltaRatio, const Graph& graph) : Request(currentRow, graph){
    _deltaTime = floor(_deltaTime * deltaRatio);
}

std::vector<Request> Request::getRequestsFromFile(const std::string& datFilePath, const Graph& graph) {
    std::vector<Request> requests;

    std::ifstream infile(datFilePath);
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

uint Request::getCurrentDeltaTime() const {
    return _currentDeltaTime;
}

uint Request::getRequestServiceStart() const {
    return _requestServiceStart;
}

uint Request::getRequestServiceEnd() const {
    return _requestServiceEnd;
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

void Request::setCurrentDeltaTime(int currentDeltaTime) {
    _currentDeltaTime = currentDeltaTime;
}

void Request::setRequestServiceStart(int requestServiceStart) {
    _requestServiceStart = requestServiceStart;
}

void Request::setRequestServiceEnd(int requestServiceEnd) {
    _requestServiceEnd = requestServiceEnd;
}

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

std::string Request::to_string_export() const {
    std::string res = std::to_string(_originNodeIndex) + "," + std::to_string(_destinationNodeIndex) //FIXME:replace with std::format whenever possible
            + "," + std::to_string(_arrivalTW.min) + "," + std::to_string(_arrivalTW.max)
            + "," + std::to_string(_deltaTime) + "," + std::to_string(_weight);
    return res;
}

Request::Request(const Graph& graph, const Request &baseRequest, const TransitAccess &access, bool isEntry) {
    if(isEntry) {
        _originNodeIndex = baseRequest.getOriginNodeIndex();
        _destinationNodeIndex = access.getAccessNodeIdx();

        _departureTW = baseRequest.getDepartureTw();

        _arrivalTW.min = baseRequest.getDepartureTw().min + graph.getShortestSAEVPath(_originNodeIndex, access.getAccessNodeIdx());
        _arrivalTW.max = access.getAccessTimestamp();
    } else {
        _originNodeIndex = access.getAccessNodeIdx();
        _destinationNodeIndex = baseRequest.getDestinationNodeIndex();

        _departureTW.min = access.getAccessTimestamp();
        _departureTW.max = baseRequest.getArrivalTw().max - graph.getShortestSAEVPath(access.getAccessNodeIdx(), _destinationNodeIndex);

        _arrivalTW.min = baseRequest.getArrivalTw().min;
        _arrivalTW.max = _departureTW.min + baseRequest.getDeltaTime(); //Reduce max arrival TW to a value we are 100% sure is compatible with our current min departure time
    }

    _deltaTime = std::numeric_limits<uint>::max();
    _weight = baseRequest.getWeight();
}
