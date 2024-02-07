//
// Created by rbernard on 29/01/2024.
// Source : https://stackoverflow.com/a/1120224

#include "../DatFile/DATRow.h"

class CSVRow : public DATRow
{
public:
    CSVRow() : DATRow(','){}
};
