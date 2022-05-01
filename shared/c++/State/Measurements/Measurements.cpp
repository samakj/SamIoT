#include "Measurements.h"

namespace SamIoT::State::Measurements
{
    char DEVICE_STATE_KEY_SEPERATOR = '|';
    char DEVICE_STATE_TAGS_SEPERATOR = ',';
    char DEVICE_STATE_KEY_EMPTY = '-';
    std::string s_DEVICE_STATE_KEY_SEPERATOR = "|";
    std::string s_DEVICE_STATE_TAGS_SEPERATOR = ",";
    std::string s_DEVICE_STATE_KEY_EMPTY = "-";

    std::unordered_map<std::string, std::nullptr_t> nullState = {};
    std::unordered_map<std::string, std::string> stringState = {};
    std::unordered_map<std::string, float> floatState = {};
    std::unordered_map<std::string, int> intState = {};
    std::unordered_map<std::string, bool> boolState = {};

    std::string getKey(std::string metric, std::vector<std::string> tags)
    {
        std::string key = "";

        if (metric.size())
            key += metric;
        else
            key += DEVICE_STATE_KEY_EMPTY;

        if (tags.size())
            key += SamIoT::Utils::join(tags, DEVICE_STATE_TAGS_SEPERATOR);
        else
            key += DEVICE_STATE_KEY_EMPTY;

        return key;
    };

    void remove(std::string metric, std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::nullState.erase(key);
        SamIoT::State::Measurements::stringState.erase(key);
        SamIoT::State::Measurements::floatState.erase(key);
        SamIoT::State::Measurements::intState.erase(key);
        SamIoT::State::Measurements::boolState.erase(key);
    };

    void *get(std::string metric, std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);

        auto stringValue = SamIoT::State::Measurements::stringState.find(key);
        if (stringValue != SamIoT::State::Measurements::stringState.end())
            return &stringValue->second;

        auto floatValue = SamIoT::State::Measurements::floatState.find(key);
        if (floatValue != SamIoT::State::Measurements::floatState.end())
            return &floatValue->second;

        auto intValue = SamIoT::State::Measurements::intState.find(key);
        if (intValue != SamIoT::State::Measurements::intState.end())
            return &intValue->second;

        auto boolValue = SamIoT::State::Measurements::boolState.find(key);
        if (boolValue != SamIoT::State::Measurements::boolState.end())
            return &boolValue->second;

        return nullptr;
    }

    void set(
        std::nullptr_t value,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::remove(key);
        SamIoT::State::Measurements::nullState[key] = value;
    };

    void set(
        bool value,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::remove(key);
        SamIoT::State::Measurements::boolState[key] = value;
    };

    void set(
        int value,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::remove(key);
        SamIoT::State::Measurements::intState[key] = value;
    };

    void set(
        float value,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::remove(key);
        SamIoT::State::Measurements::floatState[key] = value;
    };

    void set(
        std::string value,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string key = SamIoT::State::Measurements::getKey(metric, tags);
        SamIoT::State::Measurements::remove(key);
        SamIoT::State::Measurements::stringState[key] = value;
    };

    std::string serialise(std::string additionalProps)
    {
        std::string output = "[";
        uint8_t len = 0;

        for (auto &it : SamIoT::State::Measurements::nullState)
        {
            if (len)
                output += ",";
            output += SamIoT::State::Measurements::serialiseNull(it.second, it.first, additionalProps);
            len += 1;
        }

        for (auto &it : SamIoT::State::Measurements::boolState)
        {
            if (len)
                output += ",";
            output += SamIoT::State::Measurements::serialiseBool(it.second, it.first, additionalProps);
            len += 1;
        }

        for (auto &it : SamIoT::State::Measurements::intState)
        {
            if (len)
                output += ",";
            output += SamIoT::State::Measurements::serialiseInt(it.second, it.first, additionalProps);
            len += 1;
        }

        for (auto &it : SamIoT::State::Measurements::floatState)
        {
            if (len)
                output += ",";
            output += SamIoT::State::Measurements::serialiseFloat(it.second, it.first, additionalProps);
            len += 1;
        }

        for (auto &it : SamIoT::State::Measurements::stringState)
        {
            if (len)
                output += ",";
            output += SamIoT::State::Measurements::serialiseString(it.second, it.first, additionalProps);
            len += 1;
        }

        output += "]";
        return output;
    };

    std::vector<std::string> serialiseToArray(std::string additionalProps)
    {
        std::vector<std::string> output = {};

        for (auto &it : SamIoT::State::Measurements::nullState)
            output.push_back(SamIoT::State::Measurements::serialiseNull(it.second, it.first, additionalProps));

        for (auto &it : SamIoT::State::Measurements::boolState)
            output.push_back(SamIoT::State::Measurements::serialiseBool(it.second, it.first, additionalProps));

        for (auto &it : SamIoT::State::Measurements::intState)
            output.push_back(SamIoT::State::Measurements::serialiseInt(it.second, it.first, additionalProps));

        for (auto &it : SamIoT::State::Measurements::floatState)
            output.push_back(SamIoT::State::Measurements::serialiseFloat(it.second, it.first, additionalProps));

        for (auto &it : SamIoT::State::Measurements::stringState)
            output.push_back(SamIoT::State::Measurements::serialiseString(it.second, it.first, additionalProps));

        return output;
    };

    std::string serialiseKey(std::string key)
    {
        std::string output = "";
        std::vector<std::string> keySplit = SamIoT::Utils::split(key, DEVICE_STATE_KEY_SEPERATOR);

        output += "\"metric\":\"";

        if (keySplit[0] != s_DEVICE_STATE_KEY_EMPTY)
            output += keySplit[0];

        output += "\",";

        output += "\"tags\":[";

        if (keySplit[1] != s_DEVICE_STATE_KEY_EMPTY)
        {
            std::vector<std::string> tags = SamIoT::Utils::split(keySplit[1], DEVICE_STATE_TAGS_SEPERATOR);
            uint8_t i = 0;
            for (auto &it : tags)
            {
                if (i)
                    output += ",";
                output += "\"";
                output += it;
                output += "\"";
                i += 1;
            }
        }

        output += "]";

        return output;
    };

    std::string serialiseKey(std::string metric, std::vector<std::string> tags)
    {
        std::string output = "";

        output += "\"metric\":\"";

        if (metric != s_DEVICE_STATE_KEY_EMPTY)
            output += metric;

        output += "\",";

        output += "\"tags\":[";

        if (tags.size())
        {
            uint8_t i = 0;
            for (auto &it : tags)
            {
                if (i)
                    output += ",";
                output += "\"";
                output += it;
                output += "\"";
                i += 1;
            }
        }

        output += "]";

        return output;
    };

    std::string serialiseNull(
        std::nullptr_t value,
        std::string key,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(key);
        output += ",";
        output += "\"value\":null";

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseNull(
        std::nullptr_t value,
        std::string metric,
        std::vector<std::string> tags,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(metric, tags);
        output += ",";
        output += "\"value\":null";

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseBool(
        bool value,
        std::string key,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(key);
        output += ",";
        output += "\"value\":";
        output += value ? "true" : "false";

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseBool(
        bool value,
        std::string metric,
        std::vector<std::string> tags,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(metric, tags);
        output += ",";
        output += "\"value\":";
        output += value ? "true" : "false";

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseInt(
        int value,
        std::string key,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(key);
        output += ",";
        output += "\"value\":";

        char buff[64];
        sprintf(buff, "%d", value);

        output += buff;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseInt(
        int value,
        std::string metric,
        std::vector<std::string> tags,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(metric, tags);
        output += ",";
        output += "\"value\":";

        char buff[64];
        sprintf(buff, "%d", value);

        output += buff;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseFloat(
        float value,
        std::string key,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(key);
        output += ",";
        output += "\"value\":";

        char buff[64];
        sprintf(buff, "%f", value);

        output += buff;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseFloat(
        float value,
        std::string metric,
        std::vector<std::string> tags,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(metric, tags);
        output += ",";
        output += "\"value\":";

        char buff[64];
        sprintf(buff, "%f", value);

        output += buff;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseString(
        std::string value,
        std::string key,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(key);
        output += ",";
        output += "\"value\":";
        output += value;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };

    std::string serialiseString(
        std::string value,
        std::string metric,
        std::vector<std::string> tags,
        std::string additionalProps)
    {
        std::string output = "";

        output += "{";
        output += SamIoT::State::Measurements::serialiseKey(metric, tags);
        output += ",";
        output += "\"value\":";
        output += value;

        if (additionalProps.size())
        {
            output += ",";
            output += additionalProps;
        }
        output += "}";

        return output;
    };
}