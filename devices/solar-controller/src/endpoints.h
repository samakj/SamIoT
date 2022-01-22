#ifndef endpoints_h
#define endpoints_h

#include <defs.h>
#include <DeviceServer.h>
#include <string>

// 8-way board is active low.
bool INVERTED_RELAYS = true;

bool RELAY_1_NC = true;
bool RELAY_2_NC = true;
bool RELAY_3_NC = true;
bool RELAY_4_NC = true;
bool RELAY_5_NC = false;
bool RELAY_6_NC = false;
bool RELAY_7_NC = false;
bool RELAY_8_NC = false;

bool getRelayCoilState(bool output, bool nc, bool inverted)
{
    if (output && nc) return INVERTED_RELAYS;
    if (output && !nc) return !INVERTED_RELAYS;
    if (!output && nc) return !INVERTED_RELAYS;
    if (!output && !nc) return INVERTED_RELAYS;
}

bool getRelayOutputState(bool relay, bool nc, bool inverted)
{
    if (relay && INVERTED_RELAYS) return nc;
    if (relay && !INVERTED_RELAYS) return !nc;
    if (!relay && INVERTED_RELAYS) return !nc;
    if (!relay && !INVERTED_RELAYS) return nc;
}

bool getBooleanFromBodyData(uint8_t* data, size_t len, bool defaultValue = false)
{
    if (len == 1) return data[0];
    std::string value = (char*)data;
    if (value == "true") return true;
    if (value == "false") return false;
    return defaultValue;
}

void setRelay1(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay1.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_1_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay1.pinNo), RELAY_1_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay2(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay2.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_2_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay2.pinNo), RELAY_2_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay3(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay3.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_3_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay3.pinNo), RELAY_3_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay4(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay4.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_4_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay4.pinNo), RELAY_4_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay5(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay5.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_5_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay5.pinNo), RELAY_5_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay6(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay6.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_6_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay6.pinNo), RELAY_6_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay7(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay7.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_7_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay7.pinNo), RELAY_7_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}

void setRelay8(
    AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t index,
    size_t total
)
{ 
    Relay8.setPinState(
        getRelayCoilState(
            getBooleanFromBodyData(data, len),
            RELAY_8_NC,
            INVERTED_RELAYS
        )
    );
    char buffer[64];
    sprintf(
        buffer,
        "{\"on\":%s}",
        getRelayOutputState(digitalRead(Relay8.pinNo), RELAY_8_NC, INVERTED_RELAYS) ?
            "true" :
            "false"
    );
    return request->send(200, "application/json", buffer);
}


#endif