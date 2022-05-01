#include "WebServer.h"

namespace SamIoT::Server
{
    char DEVICE_STATE_KEY_SEPERATOR = '|';
    std::string s_DEVICE_STATE_KEY_SEPERATOR = "|";
    char DEVICE_STATE_TAGS_SEPERATOR = ',';
    std::string s_DEVICE_STATE_TAGS_SEPERATOR = ",";
    char DEVICE_STATE_KEY_EMPTY = '-';
    std::string s_DEVICE_STATE_KEY_EMPTY = "-";

    AsyncWebServer httpClient(80);
    AsyncWebSocket websocketClient("/ws");

    void BAD_REQUEST(AsyncWebServerRequest *request) { request->send(400); }
    void BAD_UPLOAD(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final) { request->send(400); }
    void BAD_BODY(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) { request->send(400); }
    void NOT_FOUND(AsyncWebServerRequest *request) { request->send(404, "application/json", "{\"error\":\"Route not found\"}"); }

    void rootResponse(AsyncWebServerRequest *request)
    {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html");
        request->send(response);
    }

    std::string location = "";
    std::string mac = "";
    std::string ip = "";

    bool isOnline = false;
    unsigned long lastWebsocketMessage = 0;

    void setup(bool mesh)
    {
        SamIoT::Logger::info("Starting http & websocket server.");
        SPIFFS.begin();
        SamIoT::Server::websocketClient.onEvent(SamIoT::Server::websocketEventHandler);
        DefaultHeaders::Instance().addHeader("Cache-Control", "no-cache");
        SamIoT::Server::httpClient.addHandler(&SamIoT::Server::websocketClient);
        SamIoT::Server::httpClient.on("/", HTTP_GET, SamIoT::Server::rootResponse);
        if (mesh)
        {
            SamIoT::Logger::info("Adding mesh endpoint.");
            SamIoT::Server::httpClient.on("/mesh", HTTP_GET, SamIoT::Mesh::meshResponse);
        }
        SamIoT::Server::httpClient.serveStatic("/static/", SPIFFS, "/");
        SamIoT::Server::httpClient.onNotFound(SamIoT::Server::NOT_FOUND);
        SamIoT::Server::httpClient.begin();
        SamIoT::Server::isOnline = true;
        SamIoT::Logger::info("http & websocket server started.");
    };

    void loop()
    {
        if (!SamIoT::Server::isOnline)
            SamIoT::Server::setup();
        if (
            SamIoT::Server::isOnline &&
            SamIoT::Time::millisSince(SamIoT::Server::lastWebsocketMessage) > 1000)
            SamIoT::Server::sendPing();
    };

    void addHttpEndpoint(
        const char *uri,
        WebRequestMethod method,
        ArRequestHandlerFunction onRequest,
        ArUploadHandlerFunction onUpload,
        ArBodyHandlerFunction onBody)
    {
        SamIoT::Server::httpClient.on(uri, method, onRequest, onUpload, onBody);
    };

    void setLocation(std::string _location)
    {
        SamIoT::Server::location = _location;
    };

    void setMacAddress(std::string _mac)
    {
        SamIoT::Server::mac = _mac;
    };

    void setIPAddress(std::string _ip)
    {
        SamIoT::Server::ip = _ip;
    };

    SamIoT::Server::MessageType getMessageTypeFromString(std::string type)
    {
        if (type == "measurement")
            return MEASUREMENT;
        if (type == "error")
            return ERROR;
        if (type == "ping")
            return PING;
        if (type == "meta")
            return META;
        return ERROR;
    };

    std::string getMessageTypeString(MessageType type)
    {
        switch (type)
        {
        case MEASUREMENT:
            return "measurement";
        case ERROR:
            return "error";
        case PING:
            return "ping";
        case META:
            return "meta";
        default:
            return "unknown";
        }
    };

    std::string beginMessage()
    {
        return (std::string) "{";
    };

    std::string endMessage(std::string message)
    {
        if (message.back() == ',')
            message.pop_back();
        return message + "}";
    };

    std::string getMessageMeta(
        MessageType type,
        std::string metric,
        std::vector<std::string> tags)
    {
        std::string meta = "";
        meta += "\"timestamp\":\"" + SamIoT::Time::getIsoTimestamp() + "\",";
        meta += "\"location\":\"" + SamIoT::Server::location + "\",";
        meta += "\"mac\":\"" + SamIoT::Server::mac + "\",";
        meta += "\"ip\":\"" + SamIoT::Server::ip + "\",";
        meta += "\"type\":\"" + SamIoT::Server::getMessageTypeString(type) + "\",";

        if (metric.size())
            meta += "\"metric\":\"" + metric + "\",";
        if (tags.size())
        {
            meta += "\"tags\":[";
            for (std::string tag : tags)
                meta += "\"" + tag + "\",";
            meta.pop_back();
            meta += "],";
        }
        return meta;
    };

    void sendMessage(
        std::string message,
        AsyncWebSocketClient *client)
    {
        if (client != nullptr)
        {
            if (client->canSend())
                client->text(message.c_str());
        }
        else
        {
            SamIoT::Server::websocketClient.textAll(message.c_str());
            SamIoT::Server::lastWebsocketMessage = millis();
        }
    };

    void sendPing()
    {
        std::string message = SamIoT::Server::beginMessage();
        message += SamIoT::Server::getMessageMeta(SamIoT::Server::MessageType::PING);
        message = SamIoT::Server::endMessage(message);
        sendMessage(message);
    };

    void sendState(AsyncWebSocketClient *client)
    {
        for (
            std::string &message :
            SamIoT::State::Measurements::serialiseToArray(
                SamIoT::Server::getMessageMeta(SamIoT::Server::MessageType::MEASUREMENT)))
        {
            sendMessage(message, client);
            delay(10);
        }
    };

    void websocketEventHandler(
        AsyncWebSocket *server,
        AsyncWebSocketClient *client,
        AwsEventType type,
        void *arg,
        uint8_t *data,
        size_t len)
    {
        AwsFrameInfo *info = (AwsFrameInfo *)arg;
        std::string ip = "TO-DO";

        switch (type)
        {
        case WS_EVT_CONNECT:
            SamIoT::Server::sendState(client);
            SamIoT::Logger::debugf("Client connected to websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_DISCONNECT:
            SamIoT::Logger::debugf("Client disconnected from websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_PONG:
            break;
        case WS_EVT_ERROR:
            SamIoT::Logger::debugf("Client errored on websocket from IP: %s\n", ip.c_str());
            break;
        case WS_EVT_DATA:
            if (info->opcode == WS_TEXT)
            {
                if (info->final && info->index == 0 && info->len == len)
                {
                    data[len] = 0;
                    SamIoT::Logger::warnf("%s sent text data below, skipping...\n", ip.c_str());
                    SamIoT::Logger::warn((char *)data);
                }
                else
                    SamIoT::Logger::warnf("%s sent multi-packet text data, skipping...\n", ip.c_str());
            }
            else
                SamIoT::Logger::warnf("%s sent binary data, skipping...\n", ip.c_str());
            break;
        default:
            SamIoT::Logger::warn("Unhandled websocket event type recieved...");
        }
    };
}