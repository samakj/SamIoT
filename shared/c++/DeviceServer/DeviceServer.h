#ifndef Device_Server_h
#define Device_Server_h

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
#include <sstream>
#include <vector>
#include <unordered_map>

#include <TimeUtils.h>
#include <Log.h>
#include <AsyncWifi.h>

static char DEVICE_STATE_KEY_SEPERATOR = '|';
static std::string s_DEVICE_STATE_KEY_SEPERATOR = "|";
static char DEVICE_STATE_TAGS_SEPERATOR = ',';
static std::string s_DEVICE_STATE_TAGS_SEPERATOR = ",";
static char DEVICE_STATE_KEY_EMPTY = '-';
static std::string s_DEVICE_STATE_KEY_EMPTY = "-";

enum MessageType { MEASUREMENT, ERROR, PING, META };

class DeviceState {
    public:
        std::unordered_map<std::string, std::nullptr_t> nullState = {};
        std::unordered_map<std::string, std::string> stringState = {};
        std::unordered_map<std::string, float> floatState = {};
        std::unordered_map<std::string, int> intState = {};
        std::unordered_map<std::string, bool> boolState = {};
    
    private:

    public:
        std::string buildKey(
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {},
            std::string format = ""
        );

        void print();

        void remove(
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {},
            std::string format = ""
        );

        void set(
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );

        void set(
            std::nullptr_t value,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );

        void set(
            std::string value,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );

        void set(
            bool value,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );

        void set(
            int value,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {},
            std::string format = ""
        );

        void set(
            float value,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {},
            std::string format = ""
        );
};


class DeviceServerClass {
    public:
        static void BAD_REQUEST (AsyncWebServerRequest* request);
        static void BAD_UPLOAD (AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final);
        static void BAD_BODY (AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);
        
        bool isOnline = false;
        std::string location = "";
        std::string mac = "";
        
    private:
        unsigned long lastWebsocketMessage = 0;
        DeviceState* state = nullptr;
        bool usingWifiCallback = false;

    public:
        DeviceServerClass (const DeviceServerClass&) = delete;
        static DeviceServerClass& getInstance();
        
        void setup();
        void loop();
        void addWifiConnectCallback();
        void addHttpEndpoint(
            const char* uri,
            WebRequestMethod method,
            ArRequestHandlerFunction onRequest = BAD_REQUEST,
            ArUploadHandlerFunction onUpload = BAD_UPLOAD,
            ArBodyHandlerFunction onBody = BAD_BODY
        );
        void setState(DeviceState* _state);
        void setLocation(std::string _location);

        static MessageType getMessageTypeFromString(std::string type);
        static std::string getMessageTypeString(MessageType type);
        std::string beginMessage();
        std::string addMessageMeta(
            std::string message,
            MessageType type,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        std::string addMessageValue(
            std::string message,
            std::nullptr_t value = nullptr
        );
        std::string addMessageValue(
            std::string message,
            std::string value
        );
        std::string addMessageValue(
            std::string message,
            bool value
        );
        std::string addMessageValue(
            std::string message,
            int value,
            std::string format = "%d"
        );
        std::string addMessageValue(
            std::string message,
            float value,
            std::string format = "%f"
        );
        std::string endMessage(std::string message);

        void sendMessage(
            std::string message,
            AsyncWebSocketClient* client = nullptr
        );
        void sendReport(
            MessageType type,
            std::nullptr_t value = nullptr,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendReport(
            MessageType type,
            std::string value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendReport(
            MessageType type,
            boolean value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendReport(
            MessageType type,
            int value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%d"
        );
        void sendReport(
            MessageType type,
            float value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%f"
        );
        void sendMeasurement(
            std::nullptr_t value = nullptr,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeasurement(
            std::string value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeasurement(
            boolean value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeasurement(
            int value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%d"
        );
        void sendMeasurement(
            float value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%f"
        );
        void sendError(
            std::nullptr_t value = nullptr,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendError(
            std::string value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendError(
            boolean value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendError(
            int value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%d"
        );
        void sendError(
            float value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%f"
        );
        void sendMeta(
            std::nullptr_t value = nullptr,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeta(
            std::string value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeta(
            boolean value,
            std::string metric = "",
            std::vector<std::string> tags = {}
        );
        void sendMeta(
            int value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%d"
        );
        void sendMeta(
            float value,
            std::string metric = "",
            std::vector<std::string> tags = {}, 
            std::string format = "%f"
        );
        void sendPing();
        void sendState(AsyncWebSocketClient* client = nullptr);

    private:
        DeviceServerClass() {};
        static DeviceServerClass _instance;

        void websocketEventHandler(
            AsyncWebSocket* server,
            AsyncWebSocketClient* client,
            AwsEventType type,
            void* arg,
            uint8_t* data,
            size_t len
        );
};

static DeviceServerClass& DeviceServer = DeviceServerClass::getInstance();

#endif