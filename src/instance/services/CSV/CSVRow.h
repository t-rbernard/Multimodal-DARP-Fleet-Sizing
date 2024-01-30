//
// Created by rbernard on 29/01/2024.
// Source : https://stackoverflow.com/a/1120224

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class CSVRow
{
public:
    std::string_view operator[](std::size_t index) const;
    [[nodiscard]] std::size_t size() const;
    void readNextRow(std::istream& str);
private:
    std::string         m_line;
    std::vector<int>    m_data;
};