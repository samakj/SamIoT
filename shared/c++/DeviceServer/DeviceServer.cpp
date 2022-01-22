#include <DeviceServer.h>

std::vector<std::string> split(std::string string, char delimeter = ',')
{
    std::vector<std::string> out = {};
    std::istringstream stream(string);
    int counter = 0;
    for (std::string part; std::getline(stream, part, delimeter); counter++) out.push_back(part);
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

std::string DeviceState::buildKey(
    MessageType type,
    std::string metric,
    std::vector<std::string> tags,
    std::string format
)
{

    std::string key = DeviceServerClass::getMessageTypeString(type);

    key += DEVICE_STATE_KEY_SEPERATOR + (metric.size() ? metric : s_DEVICE_STATE_KEY_EMPTY);
    key += DEVICE_STATE_KEY_SEPERATOR + (tags.size() ? join(tags, DEVICE_STATE_TAGS_SEPERATOR) : s_DEVICE_STATE_KEY_EMPTY);
    key += DEVICE_STATE_KEY_SEPERATOR + (format.size() ? format : s_DEVICE_STATE_KEY_EMPTY);

    return key;
};

void DeviceState::print()
{
    Log.info("DeviceState");
    for (auto& it: nullState) Log.infof("\t%s = null\n", it.first);
    Log.info(" ");
    for (auto& it: boolState) Log.infof("\t%s = %s\n", it.first, it.second ? "true" : "false");
    Log.info(" ");
    for (auto& it: stringState) Log.infof("\t%s = \"%s\"\n", it.first, it.second);
    Log.info(" ");
    for (auto& it: intState) Log.infof("\t%s = %d\n", it.first, it.second);
    Log.info(" ");
    for (auto& it: floatState) Log.infof("\t%s = %f\n", it.first, it.second);
}

void DeviceState::remove(
    MessageType type,
    std::string metric,
    std::vector<std::string> tags,
    std::string format
)
{
    std::string key = buildKey(type, metric, tags);
    nullState.erase(key);
    stringState.erase(key);
    floatState.erase(key);
    intState.erase(key);
    boolState.erase(key);
};

void DeviceState::set(
    MessageType type,
    std::string metric,
    std::vector<std::string> tags
)
{
    remove(type, metric, tags);
    std::string key = buildKey(type, metric, tags);
    nullState[key] = nullptr;
};

void DeviceState::set(
    std::nullptr_t value,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags
)
{
    remove(type, metric, tags);
    std::string key = buildKey(type, metric, tags);
    nullState[key] = value;
};

void DeviceState::set(
    std::string value,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags
)
{
    remove(type, metric, tags);
    std::string key = buildKey(type, metric, tags);
    stringState[key] = value;
};

void DeviceState::set(
    bool value,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags
)
{
    remove(type, metric, tags);
    std::string key = buildKey(type, metric, tags);
    boolState[key] = value;
};

void DeviceState::set(
    int value,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags,
    std::string format
)
{
    remove(type, metric, tags, format);
    std::string key = buildKey(type, metric, tags, format);
    intState[key] = value;
};

void DeviceState::set(
    float value,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags,
    std::string format
)
{
    remove(type, metric, tags, format);
    std::string key = buildKey(type, metric, tags, format);
    floatState[key] = value;
};

static AsyncWebServer http(80);
static AsyncWebSocket websocket("/ws");


DeviceServerClass DeviceServerClass::_instance;

void DeviceServerClass::BAD_REQUEST (AsyncWebServerRequest* request) { request->send(400); }
void DeviceServerClass::BAD_UPLOAD (AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) { request->send(400); }
void DeviceServerClass::BAD_BODY (AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) { request->send(400); }

DeviceServerClass& DeviceServerClass::getInstance()
{
    return _instance;
}

void rootResponse(AsyncWebServerRequest *request)
{
    AsyncWebServerResponse* response = request->beginResponse(SPIFFS, "/index.html");
    request->send(response);
}

void emptyBodyResponse(AsyncWebServerRequest* request)
{
    request->send(400, "application/json", "{\"error\":\"No body data provided\"}");
}

void notFoundResponse(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"error\":\"Route not found\"}");
}

void DeviceServerClass::setup()
{
    mac = AsyncWifi.getMACAddressString();

    Log.info("Starting http & websocket server.");
    if (AsyncWifi.isConnected())
    {
        SPIFFS.begin();
        websocket.onEvent(
            [this](AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
            { websocketEventHandler(server, client, type, arg, data, len); }
        );
        DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache");
        http.addHandler(&websocket);
        http.on("/", HTTP_GET, rootResponse);
        http.serveStatic("/static/", SPIFFS, "/");
        http.onNotFound(notFoundResponse);
        http.begin();
        isOnline = true;
    }
    else
    {
        Log.warn("Wifi not connected, failed to start server.");
    }
    Log.info("http & websocket server started.");
};

void DeviceServerClass::loop()
{
    if (!isOnline && !usingWifiCallback) setup();
    if (isOnline && TimeUtils.millisSince(lastWebsocketMessage) > 1000) sendPing();
};

void DeviceServerClass::addWifiConnectCallback()
{
    usingWifiCallback = true;
    Log.debug("Adding server Wifi connect callback.");
    AsyncWifi.addConnectCallback([this](std::string ssid) { setup(); });
};

void DeviceServerClass::addHttpEndpoint(
    const char* uri,
    WebRequestMethod method,
    ArRequestHandlerFunction onRequest,
    ArUploadHandlerFunction onUpload,
    ArBodyHandlerFunction onBody
)
{
    http.on(uri, method, onRequest, onUpload, onBody);
};

void DeviceServerClass::setState(DeviceState* _state)
{
    state = _state;
};

void DeviceServerClass::setLocation(std::string _location)
{
    location = _location;
};


MessageType DeviceServerClass::getMessageTypeFromString(std::string type)
{
    if (type == "measurement") return MEASUREMENT;
    if (type == "error") return ERROR;
    if (type == "ping") return PING;
    if (type == "meta") return META;    
    return ERROR;
};

std::string DeviceServerClass::getMessageTypeString(MessageType type)
{
    switch (type)
    {
        case MEASUREMENT: return "measurement";
        case ERROR: return "error";
        case PING: return "ping";
        case META: return "meta";
        default: return "unknown";
    }
};

std::string DeviceServerClass::beginMessage()
{
    return (std::string) "{";
};

std::string DeviceServerClass::addMessageMeta(
    std::string message,
    MessageType type,
    std::string metric,
    std::vector<std::string> tags
)
{
    message += "\"timestamp\":\"" + TimeUtils.getIsoTimestamp() + "\",";
    message += "\"location\":\"" + location + "\",";
    message += "\"mac\":\"" + mac + "\",";
    message += "\"type\":\"" + getMessageTypeString(type) + "\",";
    message += "\"metric\":\"" + metric + "\",";
    if (tags.size()) 
    {
        message += "\"tags\":[";
        for (std::string tag : tags) message += "\"" + tag + "\",";
        message.pop_back();
        message += "],";
    }
    return message;
};

std::string DeviceServerClass::addMessageValue(
    std::string message,
    std::nullptr_t value
)
{
    return message; // + "\"value\":null,";     ---- Don't bother sending this ?
};

std::string DeviceServerClass::addMessageValue(
    std::string message,
    std::string value
)
{
    return message + "\"value\":\"" + value + "\",";
};

std::string DeviceServerClass::addMessageValue(
    std::string message,
    bool value
)
{
    return message + "\"value\":\"" + (std::string)(value ? "true" : "false") + "\",";
};

std::string DeviceServerClass::addMessageValue(
    std::string message,
    int value,
    std::string format
)
{
    char valueBuffer[16];
    sprintf(valueBuffer, format.c_str(), value);
    return message + "\"value\":\"" + (std::string)(valueBuffer) + "\",";
};

std::string DeviceServerClass::addMessageValue(
    std::string message,
    float value,
    std::string format
)
{
    char valueBuffer[16];
    sprintf(valueBuffer, format.c_str(), value);
    return message + "\"value\":\"" + (std::string)(valueBuffer) + "\",";
};

std::string DeviceServerClass::endMessage(std::string message)
{
    if (message.back() == ',') message.pop_back();
    message += "}";
    return message;
};

void DeviceServerClass::sendMessage(
    std::string message,
    AsyncWebSocketClient* client
){
    if (client != nullptr) 
    {
        if (client->canSend())
        {
            client->text(message.c_str());
        }
    } 
    else 
    {
        websocket.textAll(message.c_str());
        lastWebsocketMessage = millis();
    }
};

void DeviceServerClass::sendReport(
    MessageType type,
    std::nullptr_t value,
    std::string metric,
    std::vector<std::string> tags
)
{
    std::string message = beginMessage();
    message = addMessageMeta(message, type, metric, tags);
    message = addMessageValue(message, value);
    message = endMessage(message);
    sendMessage(message);
};

void DeviceServerClass::sendReport(
    MessageType type,
    std::string value,
    std::string metric,
    std::vector<std::string> tags
)
{
    std::string message = beginMessage();
    message = addMessageMeta(message, type, metric, tags);
    message = addMessageValue(message, value);
    message = endMessage(message);
    sendMessage(message);
};

void DeviceServerClass::sendReport(
    MessageType type,
    boolean value,
    std::string metric,
    std::vector<std::string> tags
)
{
    std::string message = beginMessage();
    message = addMessageMeta(message, type, metric, tags);
    message = addMessageValue(message, value);
    message = endMessage(message);
    sendMessage(message);
};

void DeviceServerClass::sendReport(
    MessageType type,
    int value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    std::string message = beginMessage();
    message = addMessageMeta(message, type, metric, tags);
    message = addMessageValue(message, value, format);
    message = endMessage(message);
    sendMessage(message);
};

void DeviceServerClass::sendReport(
    MessageType type,
    float value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    std::string message = beginMessage();
    message = addMessageMeta(message, type, metric, tags);
    message = addMessageValue(message, value, format);
    message = endMessage(message);
    sendMessage(message);
};

void DeviceServerClass::sendMeasurement(
    std::nullptr_t value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(MEASUREMENT, value, metric, tags);
};

void DeviceServerClass::sendMeasurement(
    std::string value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(MEASUREMENT, value, metric, tags);
};

void DeviceServerClass::sendMeasurement(
    boolean value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(MEASUREMENT, value, metric, tags);
};

void DeviceServerClass::sendMeasurement(
    int value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(MEASUREMENT, value, metric, tags, format);
};

void DeviceServerClass::sendMeasurement(
    float value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(MEASUREMENT, value, metric, tags, format);
};

void DeviceServerClass::sendError(
    std::nullptr_t value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(ERROR, value, metric, tags);
};

void DeviceServerClass::sendError(
    std::string value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(ERROR, value, metric, tags);
};

void DeviceServerClass::sendError(
    boolean value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(ERROR, value, metric, tags);
};

void DeviceServerClass::sendError(
    int value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(ERROR, value, metric, tags, format);
};

void DeviceServerClass::sendError(
    float value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(ERROR, value, metric, tags, format);
};

void DeviceServerClass::sendMeta(
    std::nullptr_t value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(META, value, metric, tags);
};

void DeviceServerClass::sendMeta(
    std::string value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(META, value, metric, tags);
};

void DeviceServerClass::sendMeta(
    boolean value,
    std::string metric,
    std::vector<std::string> tags
)
{
    sendReport(META, value, metric, tags);
};

void DeviceServerClass::sendMeta(
    int value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(META, value, metric, tags, format);
};

void DeviceServerClass::sendMeta(
    float value,
    std::string metric,
    std::vector<std::string> tags, 
    std::string format
)
{
    sendReport(META, value, metric, tags, format);
};

void DeviceServerClass::sendPing()
{
    sendReport(PING);
};

void DeviceServerClass::sendState(AsyncWebSocketClient* client)
{
    for (auto& it: state->nullState) {
        std::vector<std::string> splitted = split(it.first, DEVICE_STATE_KEY_SEPERATOR);
        MessageType type = getMessageTypeFromString(splitted[0]);
        std::string metric = splitted[1] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[1];

        std::vector<std::string> tags = {};
        if (splitted[2] != s_DEVICE_STATE_KEY_EMPTY) tags = split(splitted[2], DEVICE_STATE_TAGS_SEPERATOR);

        std::string message = beginMessage();
        message = addMessageMeta(message, type, metric, tags);
        message = addMessageValue(message, it.second);
        message = endMessage(message);
        sendMessage(message, client);
    }
    for (auto& it: state->boolState) {
        std::vector<std::string> splitted = split(it.first, DEVICE_STATE_KEY_SEPERATOR);
        MessageType type = getMessageTypeFromString(splitted[0]);
        std::string metric = splitted[1] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[1];

        std::vector<std::string> tags = {};
        if (splitted[2] != s_DEVICE_STATE_KEY_EMPTY) tags = split(splitted[2], DEVICE_STATE_TAGS_SEPERATOR);

        std::string message = beginMessage();
        message = addMessageMeta(message, type, metric, tags);
        message = addMessageValue(message, it.second);
        message = endMessage(message);
        sendMessage(message, client);
    }
    for (auto& it: state->stringState) {
        std::vector<std::string> splitted = split(it.first, DEVICE_STATE_KEY_SEPERATOR);
        MessageType type = getMessageTypeFromString(splitted[0]);
        std::string metric = splitted[1] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[1];

        std::vector<std::string> tags = {};
        if (splitted[2] != s_DEVICE_STATE_KEY_EMPTY) tags = split(splitted[2], DEVICE_STATE_TAGS_SEPERATOR);

        std::string message = beginMessage();
        message = addMessageMeta(message, type, metric, tags);
        message = addMessageValue(message, it.second);
        message = endMessage(message);
        sendMessage(message, client);
    }
    for (auto& it: state->intState) {
        std::vector<std::string> splitted = split(it.first, DEVICE_STATE_KEY_SEPERATOR);
        MessageType type = getMessageTypeFromString(splitted[0]);
        std::string metric = splitted[1] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[1];
        std::string format = splitted[3] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[3];

        std::vector<std::string> tags = {};
        if (splitted[2] != s_DEVICE_STATE_KEY_EMPTY) tags = split(splitted[2], DEVICE_STATE_TAGS_SEPERATOR);

        std::string message = beginMessage();
        message = addMessageMeta(message, type, metric, tags);
        message = addMessageValue(message, it.second, format);
        message = endMessage(message);
        sendMessage(message, client);
    }
    for (auto& it: state->floatState) {
        std::vector<std::string> splitted = split(it.first, DEVICE_STATE_KEY_SEPERATOR);
        MessageType type = getMessageTypeFromString(splitted[0]);
        std::string metric = splitted[1] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[1];
        std::string format = splitted[3] == s_DEVICE_STATE_KEY_EMPTY ? "" : splitted[3];

        std::vector<std::string> tags = {};
        if (splitted[2] != s_DEVICE_STATE_KEY_EMPTY) tags = split(splitted[2], DEVICE_STATE_TAGS_SEPERATOR);

        std::string message = beginMessage();
        message = addMessageMeta(message, type, metric, tags);
        message = addMessageValue(message, it.second, format);
        message = endMessage(message);
        sendMessage(message, client);
    }
};

void DeviceServerClass::websocketEventHandler(
    AsyncWebSocket* server,
    AsyncWebSocketClient* client,
    AwsEventType type,
    void* arg,
    uint8_t* data,
    size_t len
){
    AwsFrameInfo* info = (AwsFrameInfo*)arg;
    std::string ip = AsyncWifi.IPAddressToString(client->remoteIP());

    switch(type) 
    {
        case WS_EVT_CONNECT:
            sendState(client);
            Log.debugf("Client connected to websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_DISCONNECT:
            Log.debugf("Client disconnected from websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_PONG:
            break;
        case WS_EVT_ERROR:
            Log.debugf("Client errored on websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_DATA:
            if (info->opcode == WS_TEXT)
            {
                if(info->final && info->index == 0 && info->len == len)
                {
                    data[len] = 0;
                    Log.warnf("%s sent text data below, skipping...\n", ip.c_str());
                    Log.warn((char*)data);
                }
                else Log.warnf("%s sent multi-packet text data, skipping...\n", ip.c_str());
            }
            else Log.warnf("%s sent binary data, skipping...\n", ip.c_str());
            break;
        default:
            Log.warn("Unhandled websocket event type recieved...");
    }
};