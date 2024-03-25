//
// Created by rbernard on 29/01/2024.
// Source : https://stackoverflow.com/a/1120224

#ifndef GREEDYALGORITHM_CSVRANGE_H
#define GREEDYALGORITHM_CSVRANGE_H


#include <istream>
#include "CSVIterator.h"

class CSVRange {
    std::istream&   stream;
public:
    CSVRange(std::istream& str)
            : stream(str)
    {}
    CSVIterator begin() const {return CSVIterator{stream};}
    CSVIterator end()   const {return CSVIterator{};}
};


#endif //GREEDYALGORITHM_CSVRANGE_H
