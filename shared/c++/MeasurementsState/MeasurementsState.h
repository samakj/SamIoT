#ifndef MeasurementsState_h
#define MeasurementsState_h

#include <Arduino.h>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class MeasurementState
{
public:
private:
    static char DEVICE_STATE_KEY_SEPERATOR = '|';
    static char DEVICE_STATE_TAGS_SEPERATOR = ',';
    static char DEVICE_STATE_KEY_EMPTY = '-';

    std::unordered_map<std::string, std::nullptr_t> nullState = {};
    std::unordered_map<std::string, std::string> stringState = {};
    std::unordered_map<std::string, float> floatState = {};
    std::unordered_map<std::string, int> intState = {};
    std::unordered_map<std::string, bool> boolState = {};

public:
    void remove(std::string metric = "", std::vector<std::string> tags = {});

    void *get(std::string metric = "", std::vector<std::string> tags = {});
    std::string getKey(std::string metric = "", std::vector<std::string> tags = {});

    void set(std::nullptr value = nullptr, std::string metric = "", std::vector<std::string> tags = {});
    void set(bool value, std::string metric = "", std::vector<std::string> tags = {});
    void set(int value, std::string metric = "", std::vector<std::string> tags = {});
    void set(float value, std::string metric = "", std::vector<std::string> tags = {});
    void set(std::string value, std::string metric = "", std::vector<std::string> tags = {});

    std::string serialise(std::string additionalProps = "");
    std::string serialiseKey(std::string key);
    std::string serialiseKey(std::string metric = "", std::vector<std::string> tags = {});
    std::string serialiseNull(std::nullptr value = nullptr, std::string key = "", std::string additionalProps = "");
    std::string serialiseNull(std::nullptr value = nullptr, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "");
    std::string serialiseBool(bool value, std::string key = "", std::string additionalProps = "");
    std::string serialiseBool(bool value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "");
    std::string serialiseInt(int value, std::string key = "", std::string additionalProps = "");
    std::string serialiseInt(int value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "");
    std::string serialiseFloat(float value, std::string key = "", std::string additionalProps = "");
    std::string serialiseFloat(float value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "");
    std::string serialiseString(std::string value, std::string key = "", std::string additionalProps = "");
    std::string serialiseString(std::string value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "");

private:
};

#endif