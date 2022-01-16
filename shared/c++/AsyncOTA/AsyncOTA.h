#ifndef AsyncOTA_h
#define AsyncOTA_h

#include <ArduinoOTA.h>
#include <string>
#include <Log.h>
#include <TelnetStream.h>

class AsyncOTAClass
{
    public:

    private:

    public:
        AsyncOTAClass (const AsyncOTAClass&) = delete;
        static AsyncOTAClass& getInstance();

        void setup(std::string hostname, std::string password);
        void loop();

    private:
        AsyncOTAClass() {};
        static AsyncOTAClass _instance;
};

static AsyncOTAClass& AsyncOTA = AsyncOTAClass::getInstance();

#endif