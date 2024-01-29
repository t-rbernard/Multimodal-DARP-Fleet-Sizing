cmake_minimum_required(VERSION 3.27)
project(GreedyAlgorithm)

set(CMAKE_CXX_STANDARD 23)

add_executable(GreedyAlgorithm src/main.cpp
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
        src/instance/service/CSVRow.cpp
        src/instance/service/CSVRange.cpp
        src/instance/service/CSVRange.h
        src/instance/service/CSVIterator.cpp
        src/instance/service/CSVIterator.h
)
