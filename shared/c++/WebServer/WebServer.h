#ifndef _SamIoT_WebServer_h
#define _SamIoT_WebServer_h

#include <Arduino.h>

#ifdef ESP8266
#include <ESPAsyncTCP.h>
#include <FS.h>
#else
#include <AsyncTCP.h>
#include <SPIFFS.h>
#endif

#include <ESPAsyncWebServer.h>
#include <string>
#include <unordered_map>
#include <vector>

#include <Logger.h>
#include <Mesh.h>
#include <State.h>
#include <Time.h>

namespace SamIoT::Server
{
    extern char DEVICE_STATE_KEY_SEPERATOR;
    extern std::string s_DEVICE_STATE_KEY_SEPERATOR;
    extern char DEVICE_STATE_TAGS_SEPERATOR;
    extern std::string s_DEVICE_STATE_TAGS_SEPERATOR;
    extern char DEVICE_STATE_KEY_EMPTY;
    extern std::string s_DEVICE_STATE_KEY_EMPTY;

    enum MessageType
    {
        MEASUREMENT,
        ERROR,
        PING,
        META
    };

    extern AsyncWebServer httpClient;
    extern AsyncWebSocket websocketClient;

    void BAD_REQUEST(AsyncWebServerRequest *request);
    void BAD_UPLOAD(AsyncWebServerRequest *request, const String &filename, size_t index, uint8_t *data, size_t len, bool final);
    void BAD_BODY(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
    void NOT_FOUND(AsyncWebServerRequest *request);

    void rootResponse(AsyncWebServerRequest *request);

    extern std::string location;
    extern std::string mac;
    extern std::string ip;

    extern bool isOnline;
    extern unsigned long lastWebsocketMessage;

    void setup(bool mesh = false);
    void loop();

    void addHttpEndpoint(
        const char *uri,
        WebRequestMethod method,
        ArRequestHandlerFunction onRequest = SamIoT::Server::BAD_REQUEST,
        ArUploadHandlerFunction onUpload = SamIoT::Server::BAD_UPLOAD,
        ArBodyHandlerFunction onBody = SamIoT::Server::BAD_BODY);

    void setLocation(std::string location);
    void setMacAddress(std::string mac);
    void setIPAddress(std::string ip);

    MessageType getMessageTypeFromString(std::string type);
    std::string getMessageTypeString(MessageType type);

    std::string beginMessage();
    std::string endMessage(std::string message);

    std::string getMessageMeta(
        MessageType type,
        std::string metric = "",
        std::vector<std::string> tags = {});

    void sendMessage(
        std::string message,
        AsyncWebSocketClient *client = nullptr);
    void sendPing();
    void sendState(AsyncWebSocketClient *client = nullptr);

    void websocketEventHandler(
        AsyncWebSocket *server,
        AsyncWebSocketClient *client,
        AwsEventType type,
        void *arg,
        uint8_t *data,
        size_t len);
};

#endif