#include "display.h"

namespace Display
{
    TFT_eSPI tft;

    SamIoT::Graphics::Navigation Navigation;
    SamIoT::Graphics::RendererClass *Renderer = &SamIoT::Graphics::Renderer;

    void setup()
    {
        Display::tft.init();
        Display::tft.fillScreen(TFT_BLACK);
        Display::tft.setRotation(2);
        Display::Navigation.addPage(Display::Views::Boot::name, &Display::Views::Boot::elements);
        Display::Navigation.addPage(Display::Views::Main::name, &Display::Views::Main::elements);
    }

    namespace Views
    {
        namespace Boot
        {
            std::string name = "BOOT";

            SamIoT::Graphics::Elements::Text Title(
                &Display::tft,
                TFT_WIDTH / 2,
                TFT_HEIGHT / 2,
                "Booting...",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MC_DATUM);

            SamIoT::Graphics::Elements::Text Task(
                &Display::tft,
                TFT_WIDTH / 2,
                TFT_HEIGHT / 2 + 48,
                "...",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MC_DATUM);

            std::vector<SamIoT::Graphics::Elements::Element *> elements = {
                &Display::Views::Boot::Title,
                &Display::Views::Boot::Task};

            void updateTask(std::string task)
            {
                Display::Views::Boot::Task.setText(task);
            }
        }
        namespace Main
        {
            std::string name = "MAIN";

            SamIoT::Graphics::Elements::Time Time(
                &Display::tft,
                TFT_WIDTH - 8,
                8,
                "--",
                "--",
                "--",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                TR_DATUM);

            SamIoT::Graphics::Elements::Text Wifi(
                &Display::tft,
                8,
                8,
                "---- --%",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                TL_DATUM);

            SamIoT::Graphics::Elements::Text CoopTitle(
                &Display::tft,
                8,
                80,
                "Coop",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                ML_DATUM);

            SamIoT::Graphics::Elements::Text CoopTemperature(
                &Display::tft,
                TFT_WIDTH - 16,
                76,
                "--.-c",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            SamIoT::Graphics::Elements::Text CoopHumidity(
                &Display::tft,
                TFT_WIDTH - 16,
                104,
                "--.-%",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            SamIoT::Graphics::Elements::Text ShadeTitle(
                &Display::tft,
                8,
                170,
                "Shade",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                ML_DATUM);

            SamIoT::Graphics::Elements::Text ShadeTemperature(
                &Display::tft,
                TFT_WIDTH - 16,
                166,
                "--.-c",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            SamIoT::Graphics::Elements::Text ShadeHumidity(
                &Display::tft,
                TFT_WIDTH - 16,
                194,
                "--.-%",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            SamIoT::Graphics::Elements::Text RunTitle(
                &Display::tft,
                8,
                250,
                "Run",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                ML_DATUM);

            SamIoT::Graphics::Elements::Text RunTemperature(
                &Display::tft,
                TFT_WIDTH - 16,
                246,
                "--.-c",
                &FreeSans18pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            SamIoT::Graphics::Elements::Text RunHumidity(
                &Display::tft,
                TFT_WIDTH - 16,
                274,
                "--.-%",
                &FreeSans9pt7b,
                TFT_WHITE,
                TFT_BLACK,
                MR_DATUM);

            std::vector<SamIoT::Graphics::Elements::Element *> elements = {
                &Display::Views::Main::Time,
                &Display::Views::Main::Wifi,
                &Display::Views::Main::CoopTitle,
                &Display::Views::Main::CoopTemperature,
                &Display::Views::Main::CoopHumidity,
                &Display::Views::Main::ShadeTitle,
                &Display::Views::Main::ShadeTemperature,
                &Display::Views::Main::ShadeHumidity,
                &Display::Views::Main::RunTitle,
                &Display::Views::Main::RunTemperature,
                &Display::Views::Main::RunHumidity};

            void updateTime()
            {
                if (SamIoT::Time::NTP::isConnected)
                    Time.setFromTime();
            }

            void updateWifi(std::string ssid, float strength)
            {
                std::string wifiText = "";

                if (ssid == SamIoT::Wifi::SSID_NULL_VALUE)
                {
                    wifiText += "----";
                }
                else
                {
                    char ssidText[24];
                    sprintf(ssidText, "%s", ssid.c_str());
                    wifiText += ssidText;
                }
                wifiText += " ";

                if (strength == SamIoT::Wifi::STRENGTH_NULL_VALUE)
                {
                    wifiText += "--%";
                }
                else
                {
                    char strengthText[24];
                    sprintf(strengthText, "%.0f%%", strength);
                    wifiText += strengthText;
                }

                Display::Views::Main::Wifi.setText(wifiText);
            }

            void updateCoopTemperature(float temperature)
            {
                std::string temperatureText = "";

                if (temperature == SamIoT::Sensors::TEMPERATURE_NULL_VALUE)
                {
                    temperatureText += "--.-c";
                    Display::Views::Main::CoopTemperature.setColor(TFT_WHITE);
                }
                else
                {
                    char _temperatureText[24];
                    sprintf(_temperatureText, "%.1fc", temperature);
                    temperatureText += _temperatureText;
                    if (temperature > 30)
                        Display::Views::Main::CoopTemperature.setColor(TFT_RED);
                    else if (temperature < 5)
                        Display::Views::Main::CoopTemperature.setColor(TFT_CYAN);
                    else
                        Display::Views::Main::CoopTemperature.setColor(TFT_WHITE);
                }
                Display::Views::Main::CoopTemperature.setText(temperatureText);
            };

            void updateCoopHumidity(float humidity)
            {
                std::string humidityText = "";

                if (humidity == SamIoT::Sensors::HUMIDITY_NULL_VALUE)
                {
                    humidityText += "--.-%";
                }
                else
                {
                    char _humidityText[24];
                    sprintf(_humidityText, "%.1f%%", humidity);
                    humidityText += _humidityText;
                }
                Display::Views::Main::CoopHumidity.setText(humidityText);
            };

            void updateShadeTemperature(float temperature)
            {
                std::string temperatureText = "";

                if (temperature == SamIoT::Sensors::TEMPERATURE_NULL_VALUE)
                {
                    temperatureText += "--.-c";
                    Display::Views::Main::ShadeTemperature.setColor(TFT_WHITE);
                }
                else
                {
                    char _temperatureText[24];
                    sprintf(_temperatureText, "%.1fc", temperature);
                    temperatureText += _temperatureText;
                    if (temperature > 30)
                        Display::Views::Main::ShadeTemperature.setColor(TFT_RED);
                    else if (temperature < 5)
                        Display::Views::Main::ShadeTemperature.setColor(TFT_CYAN);
                    else
                        Display::Views::Main::ShadeTemperature.setColor(TFT_WHITE);
                }
                Display::Views::Main::ShadeTemperature.setText(temperatureText);
            };

            void updateShadeHumidity(float humidity)
            {
                std::string humidityText = "";

                if (humidity == SamIoT::Sensors::HUMIDITY_NULL_VALUE)
                {
                    humidityText += "--.-%";
                }
                else
                {
                    char _humidityText[24];
                    sprintf(_humidityText, "%.1f%%", humidity);
                    humidityText += _humidityText;
                }
                Display::Views::Main::ShadeHumidity.setText(humidityText);
            };

            void updateRunTemperature(float temperature)
            {
                std::string temperatureText = "";

                if (temperature == SamIoT::Sensors::TEMPERATURE_NULL_VALUE)
                {
                    temperatureText += "--.-c";
                    Display::Views::Main::RunTemperature.setColor(TFT_WHITE);
                }
                else
                {
                    char _temperatureText[24];
                    sprintf(_temperatureText, "%.1fc", temperature);
                    temperatureText += _temperatureText;
                    if (temperature > 30)
                        Display::Views::Main::RunTemperature.setColor(TFT_RED);
                    else if (temperature < 5)
                        Display::Views::Main::RunTemperature.setColor(TFT_CYAN);
                    else
                        Display::Views::Main::RunTemperature.setColor(TFT_WHITE);
                }
                Display::Views::Main::RunTemperature.setText(temperatureText);
            };

            void updateRunHumidity(float humidity)
            {
                std::string humidityText = "";

                if (humidity == SamIoT::Sensors::HUMIDITY_NULL_VALUE)
                {
                    humidityText += "--.-%";
                }
                else
                {
                    char _humidityText[24];
                    sprintf(_humidityText, "%.1f%%", humidity);
                    humidityText += _humidityText;
                }
                Display::Views::Main::RunHumidity.setText(humidityText);
            };

        }
    }
}