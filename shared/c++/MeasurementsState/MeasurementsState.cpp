#include "MeasurementsState.h"

std::vector<std::string> split(std::string string, char delimeter = ',')
{
    std::vector<std::string> out = {};
    std::istringstream stream(string);
    int counter = 0;
    for (std::string part; std::getline(stream, part, delimeter); counter++)
        out.push_back(part);
    return out;
}

std::string join(std::vector<std::string> list, char delimeter = ',')
{
    std::string out = "";
    for (std::string part : list)
    {
        out += part;
        out += delimeter;
    }
    out.pop_back();
    return out;
}

std::string MeasurementsState::getdKey(std::string metric, std::vector<std::string> tags)
{
    std::string key = "";

    if (metric.size())
        key += metric;
    else
        key += DEVICE_STATE_KEY_EMPTY;

    if (tags.size())
        key += join(tags, DEVICE_STATE_TAGS_SEPERATOR);
    else
        key += DEVICE_STATE_KEY_EMPTY;

    return key;
};

void MeasurementsState::remove(std::string metric, std::vector<std::string> tags)
{
    std::string key = getKey(metric, tags);
    nullState.erase(key);
    stringState.erase(key);
    floatState.erase(key);
    intState.erase(key);
    boolState.erase(key);
};

void *MeasurementsState::get(std::string metric, std::vector<std::string> tags)
{
    std::string key = getKey(metric, tags);

    auto stringValue = stringState.find(key);
    if (stringValue != stringState.end())
        return stringValue->second;

    auto floatValue = floatState.find(key);
    if (floatValue != floatState.end())
        return floatValue->second;

    auto intValue = intState.find(key);
    if (intValue != intState.end())
        return intValue->second;

    auto boolValue = boolState.find(key);
    if (boolValue != boolState.end())
        return boolValue->second;

    return nullptr
}

void MeasurementsState::set(
    std::nullptr value = nullptr,
    std::string metric = "",
    std::vector<std::string> tags = {})
{
    std::string key = getKey(metric, tags);
    remove(key);
    nullState[key] = value
};

void MeasurementsState::set(
    bool value,
    std::string metric = "",
    std::vector<std::string> tags = {})
{
    std::string key = getKey(metric, tags);
    remove(key);
    boolState[key] = value
};

void MeasurementsState::set(
    int value,
    std::string metric = "",
    std::vector<std::string> tags = {})
{
    std::string key = getKey(metric, tags);
    remove(key);
    intState[key] = value
};

void MeasurementsState::set(
    float value,
    std::string metric = "",
    std::vector<std::string> tags = {})
{
    std::string key = getKey(metric, tags);
    remove(key);
    floatState[key] = value
};

void MeasurementsState::set(
    std::string value,
    std::string metric = "",
    std::vector<std::string> tags = {})
{
    std::string key = getKey(metric, tags);
    remove(key);
    stringState[key] = value
};

std::string MeasurementsState::serialise(std::string additionalProps = "")
{
    std::string output = "[";
    int len = 0;

    for (auto &it : nullState)
    {
        if (len)
            output += ",";
        output += serialiseNull(it->second, it->first, additionalProps);
        len += 1;
    }

    for (auto &it : boolState)
    {
        if (len)
            output += ",";
        output += serialiseBool(it->second, it->first, additionalProps);
        len += 1;
    }

    for (auto &it : intState)
    {
        if (len)
            output += ",";
        output += serialiseInt(it->second, it->first, additionalProps);
        len += 1;
    }

    for (auto &it : floatState)
    {
        if (len)
            output += ",";
        output += serialiseFloat(it->second, it->first, additionalProps);
        len += 1;
    }

    for (auto &it : stringState)
    {
        if (len)
            output += ",";
        output += serialiseString(it->second, it->first, additionalProps);
        len += 1;
    }

    output += "]";
    return output;
};

std::string MeasurementsState::serialiseKey(std::string key)
{
    std::string output = "";
    std::vector<std::string> keySpilt = split(key, DEVICE_STATE_KEY_SEPERATOR);

    output += "\"metric\":\"";

    if (keySplit[0] != DEVICE_STATE_KEY_EMPTY)
        output += keySplit[0];

    output += "\",";

    output += "\"tags\":[";

    if (keySplit[1] != DEVICE_STATE_KEY_EMPTY)
    {
        std::vector<std::string> tags = split(keySplit[1], DEVICE_STATE_TAGS_SEPERATOR);
        int i = 0;
        for (auto &it : tags)
        {
            if (i)
                output += ",";
            output += "\"";
            output += it->first;
            output += "\"";
            i += 1
        }
    }

    output += "]";

    return output;
};

std::string MeasurementsState::serialiseKey(std::string metric = "", std::vector<std::string> tags = {})
{
    std::string output = "";

    output += "\"metric\":\"";

    if (metric != DEVICE_STATE_KEY_EMPTY)
        output += metric;

    output += "\",";

    output += "\"tags\":[";

    if (tags.size())
    {
        std::vector<std::string> tags = split(keySplit[1], DEVICE_STATE_TAGS_SEPERATOR);
        int i = 0;
        for (auto &it : tags)
        {
            if (i)
                output += ",";
            output += "\"";
            output += it->first;
            output += "\"";
            i += 1
        }
    }

    output += "]";

    return output;
};

std::string MeasurementsState::serialiseNull(std::nullptr value = nullptr, std::string key = "", std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(key);
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

std::string MeasurementsState::serialiseNull(std::nullptr value = nullptr, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(metric, tags);
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

std::string MeasurementsState::serialiseBool(bool value, std::string key = "", std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(key);
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

std::string MeasurementsState::serialiseBool(bool value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(metric, tags);
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

std::string MeasurementsState::serialiseInt(int value, std::string key = "", std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(key);
    output += ",";
    output += "\"value\":";

    char buff[64];
    sprintf("%d", it->second);

    output += buff;

    if (additionalProps.size())
    {
        output += ",";
        output += additionalProps;
    }
    output += "}";

    return output;
};

std::string MeasurementsState::serialiseInt(int value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(metric, tags);
    output += ",";
    output += "\"value\":";

    char buff[64];
    sprintf("%d", it->second);

    output += buff;

    if (additionalProps.size())
    {
        output += ",";
        output += additionalProps;
    }
    output += "}";

    return output;
};

std::string MeasurementsState::serialiseFloat(float value, std::string key = "", std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(key);
    output += ",";
    output += "\"value\":";

    char buff[64];
    sprintf("%f", it->second);

    output += buff;

    if (additionalProps.size())
    {
        output += ",";
        output += additionalProps;
    }
    output += "}";

    return output;
};

std::string MeasurementsState::serialiseFloat(float value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(metric, tags);
    output += ",";
    output += "\"value\":";

    char buff[64];
    sprintf("%f", it->second);

    output += buff;

    if (additionalProps.size())
    {
        output += ",";
        output += additionalProps;
    }
    output += "}";

    return output;
};

std::string MeasurementsState::serialiseString(std::string value, std::string key = "", std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(key);
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

std::string MeasurementsState::serialiseString(std::string value, std::string metric = "", std::vector<std::string> tags = {}, std::string additionalProps = "")
{
    std::string output = "";

    output += "{";
    output += serialiseKey(metric, tags);
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
