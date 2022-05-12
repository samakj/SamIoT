#ifndef display_h
#define display_h

#include <Graphics.h>
#include <TFT_eSPI.h>
#include <Wifi.h>
#include <string>

#include <Logger.h>
#include <Sensors.h>

namespace Display
{
    extern TFT_eSPI tft;

    extern SamIoT::Graphics::Navigation Navigation;
    extern SamIoT::Graphics::RendererClass *Renderer;

    namespace Views
    {
        namespace Boot
        {
            extern std::string name;
            extern SamIoT::Graphics::Elements::Text Title;
            extern SamIoT::Graphics::Elements::Text Task;
            extern std::vector<SamIoT::Graphics::Elements::Element *> elements;

            void updateTask(std::string task);
        }

        namespace Main
        {
            extern std::string name;
            extern SamIoT::Graphics::Elements::Time Time;
            extern SamIoT::Graphics::Elements::Text Wifi;
            extern SamIoT::Graphics::Elements::Text CoopTitle;
            extern SamIoT::Graphics::Elements::Text CoopTemperature;
            extern SamIoT::Graphics::Elements::Text CoopHumidity;
            extern SamIoT::Graphics::Elements::Text ShadeTitle;
            extern SamIoT::Graphics::Elements::Text ShadeTemperature;
            extern SamIoT::Graphics::Elements::Text ShadeHumidity;
            extern SamIoT::Graphics::Elements::Text RunTitle;
            extern SamIoT::Graphics::Elements::Text RunTemperature;
            extern SamIoT::Graphics::Elements::Text RunHumidity;
            extern std::vector<SamIoT::Graphics::Elements::Element *> elements;

            void updateTime();
            void updateWifi(std::string ssid, float strength);
            void updateCoopTemperature(float temperature);
            void updateCoopHumidity(float humidity);
            void updateShadeTemperature(float temperature);
            void updateShadeHumidity(float humidity);
            void updateRunTemperature(float temperature);
            void updateRunHumidity(float humidity);
        }
    };

    void setup();
}

#endif