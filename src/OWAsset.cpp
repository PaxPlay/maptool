#include "OWAsset.h"

#include <fstream>

std::string OWAsset::readString(std::ifstream &ifstream)
{
    uint8_t length = 0;
    ifstream.read((char*)&length, 1);
    if (length == 0)
        return {};
    char *value = new char[length + 1];
    ifstream.read(value, length);
    value[length] = 0;
    std::string ret { value };
    delete[] value;

    return ret;
}
