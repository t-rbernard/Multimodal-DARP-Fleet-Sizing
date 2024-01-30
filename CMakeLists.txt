cmake_minimum_required(VERSION 3.27)
project(GreedyAlgorithm)

set(CMAKE_CXX_STANDARD 23)

add_executable(GreedyAlgorithm
        src/instance/graph/Node.cpp
        src/instance/graph/Node.h
        src/instance/graph/Edge.cpp
        src/instance/graph/Edge.h
        src/instance/graph/Graph.cpp
        src/instance/graph/Graph.h
        src/instance/requests/Request.cpp
        src/instance/requests/Request.h
        src/instance/Instance.cpp
        src/instance/Instance.h
        src/instance/graph/Line.cpp
        src/instance/graph/Line.h
        src/instance/services/CSV/CSVRow.cpp
        src/instance/services/CSV/CSVRange.cpp
        src/instance/services/CSV/CSVRange.h
        src/instance/services/CSV/CSVIterator.cpp
        src/instance/services/CSV/CSVIterator.h
        test/debug.cpp
        src/instance/graph/LineStop.cpp
        src/instance/graph/LineStop.h
)
