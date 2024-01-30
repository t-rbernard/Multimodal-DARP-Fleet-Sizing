//
// Created by rbernard on 29/01/2024.
// Source : https://stackoverflow.com/a/1120224

#ifndef GREEDYALGORITHM_CSVITERATOR_H
#define GREEDYALGORITHM_CSVITERATOR_H


#include "CSVRow.h"

class CSVIterator {
    typedef std::input_iterator_tag     iterator_category;
    typedef CSVRow                      value_type;
    typedef std::size_t                 difference_type;
    typedef CSVRow*                     pointer;
    typedef CSVRow&                     reference;

public:
    CSVIterator(std::istream& str)  :m_str(str.good()?&str:nullptr) { ++(*this); }
    CSVIterator()                   :m_str(nullptr) {}

    // Pre Increment
    CSVIterator& operator++()               {if (m_str) { if (!((*m_str) >> m_row)){m_str = nullptr;}}return *this;}
    // Post increment
    CSVIterator operator++(int)             {CSVIterator    tmp(*this);++(*this);return tmp;}
    CSVRow const& operator*()   const       {return m_row;}
    CSVRow const* operator->()  const       {return &m_row;}

    bool operator==(CSVIterator const& rhs) {return ((this == &rhs) || ((this->m_str == nullptr) && (rhs.m_str == nullptr)));}
    bool operator!=(CSVIterator const& rhs) {return !((*this) == rhs);}
private:
    std::istream*       m_str;
    CSVRow              m_row;
};


#endif //GREEDYALGORITHM_CSVITERATOR_H
