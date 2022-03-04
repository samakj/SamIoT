#ifndef tags_h
#define tags_h

#include <string>
#include <vector>

std::vector<std::string> WIFI_TAGS = {"wifi"};
std::vector<std::string> NORTH_BED_TEMPERATURE_TAGS = {"north-bed", "environment"};
std::vector<std::string> NORTH_BED_HUMIDITY_TAGS = {"north-bed", "environment"};
std::vector<std::string> NORTH_BED_DEPTH_TAGS = {"north-bed", "tank"};
std::vector<std::string> SOUTH_BED_TEMPERATURE_TAGS = {"south-bed", "environment"};
std::vector<std::string> SOUTH_BED_HUMIDITY_TAGS = {"south-bed", "environment"};
std::vector<std::string> SOUTH_BED_DEPTH_TAGS = {"south-bed", "tank"};

#endif