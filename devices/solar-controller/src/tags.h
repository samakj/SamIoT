#ifndef tags_h
#define tags_h

#include <string>
#include <vector>

std::vector<std::string> WIFI_TAGS = {"wifi"};
std::vector<std::string> TEMPERATURE_TAGS = {"environment"};
std::vector<std::string> HUMIDITY_TAGS = {"environment"};
std::vector<std::string> PV_VOLTAGE_TAGS = {"solar", "panel"};
std::vector<std::string> PV_CURRENT_TAGS = {"solar", "panel"};
std::vector<std::string> PV_POWER_TAGS = {"solar", "panel"};
std::vector<std::string> BATTERY_CHARGING_VOLTAGE_TAGS = {"solar", "battery"};
std::vector<std::string> BATTERY_CHARGING_CURRENT_TAGS = {"solar", "battery"};
std::vector<std::string> BATTERY_CHARGING_POWER_TAGS = {"solar", "battery"};
std::vector<std::string> LOAD_VOLTAGE_TAGS = {"solar", "load"};
std::vector<std::string> LOAD_CURRENT_TAGS = {"solar", "load"};
std::vector<std::string> LOAD_POWER_TAGS = {"solar", "load"};
std::vector<std::string> BATTERY_TEMPERATURE_TAGS = {"solar", "battery"};
std::vector<std::string> CASE_TEMPERATURE_TAGS = {"solar", "case"};
std::vector<std::string> HEAT_SINK_TEMPERATURE_TAGS = {"solar", "heat-sink"};
std::vector<std::string> BATTERY_PERCENTAGE_TAGS = {"solar", "battery"};
std::vector<std::string> REMOTE_BATTERY_TEMPERATURE_TAGS = {"solar", "battery", "remote"};
std::vector<std::string> SYSTEM_VOLTAGE_TAGS = {"solar", "system"};

#endif