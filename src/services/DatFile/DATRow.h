//
// Created by rbernard on 29/01/2024.
// Adapted from : https://stackoverflow.com/a/1120224

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class DATRow
{
public:
    std::string_view operator[](std::size_t index) const
    {
        return std::string_view(&m_line[m_data[index] + 1], m_data[index + 1] -  (m_data[index] + 1));
    }

    [[nodiscard]] std::size_t size() const
    {
        return m_data.size() - 1;
    }

    void readNextRow(std::istream& str)
    {
        std::getline(str, m_line);

        m_data.clear();
        m_data.emplace_back(-1);
        std::string::size_type pos = 0;
        while((pos = m_line.find(_separator, pos)) != std::string::npos)
        {
            m_data.emplace_back(pos);
            ++pos;
        }
        // This checks for a trailing comma with no data after it.
        pos   = m_line.size();
        m_data.emplace_back(pos);
    }

    std::string toString() {return m_line;}

    DATRow() : _separator(' ') {}
    explicit DATRow(const char separator) : _separator(separator) {}
protected:
    const char          _separator;
    std::string         m_line;
    std::vector<int>    m_data;
};

inline std::istream& operator>>(std::istream& str, DATRow& data)
{
    data.readNextRow(str);
    return str;
}