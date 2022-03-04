#ifndef _SamIoT_Utils_h
#define _SamIoT_Utils_h

#include <Arduino.h>
#include <sstream>
#include <string>

namespace SamIoT::Utils
{
    std::vector<std::string> split(std::string string, char delimeter = ',');
    std::string join(std::vector<std::string> list, char delimeter = ',');
}

#endif