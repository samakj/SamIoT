#ifndef _SamIoT_Graphics_Elements_Time_h
#define _SamIoT_Graphics_Elements_Time_h

#include <string>
#include <vector>

#include "Text.h"

namespace SamIot::Graphics::Elements {
    class TimeElement: public SamIot::Graphics::Elements::Element
    {
        public:
            std::string hours = "--";
            std::string minutes = "--";
            std::string seconds = "--";
            const GFXfont* font = nullptr;
            int16_t color = TFT_WHITE;
            int16_t background = TFT_BLACK;
            uint8_t datum = TL_DATUM;
        
        protected:
            std::string displayedHours = "--";
            std::string displayedMinutes = "--";
            std::string displayedSeconds = "--";
            const GFXfont* displayedFont = nullptr;
            int16_t displayedColor = TFT_WHITE;
            int16_t displayedBackground = TFT_BLACK;
            uint8_t displayedDatum = TL_DATUM;
            int16_t maxPartWidth = 0;
            int16_t colonPadding = 2;
            int16_t colonWidth = 0;

            std::vector<SamIot::Graphics::Elements::Text> elements = {};

        public:
            Time(
                TFT_eSPI* tft = nullptr,
                int16_t x = 0, 
                int16_t y = 0,
                std::string hours = "--",
                std::string minutes = "--",
                std::string seconds = "--",
                const GFXfont* font = nullptr,
                int16_t color = TFT_WHITE,
                int16_t background = TFT_BLACK,
                uint8_t datum = TL_DATUM,
                uint16_t zIndex = 1,
                bool isManaged = false
            );

            void setX(int16_t x);
            void setY(int16_t y);
            void setHours(std::string hours = "--");
            void setHours(uint8_t hours);
            void setMinutes(std::string minutes = "--");
            void setMinutes(uint8_t minutes);
            void setSeconds(std::string seconds = "--");
            void setSeconds(uint8_t seconds);
            void setFromTime();
            void setFont(const GFXfont* font = nullptr);
            void setColor(int16_t color = TFT_WHITE);
            void setBackground(int16_t background = TFT_BLACK);
            void setDatum(uint8_t datum = TL_DATUM);
            void setVisible(bool visible = true);

            SamIot::Graphics::Point getDimensions();
            SamIot::Graphics::Point getDisplayedDimensions();
            SamIot::Graphics::Box getBounds();
            SamIot::Graphics::Box getDisplayedBounds();

            void configureTFTFontStyles();

            bool needsRedraw();
            void draw();
        
        protected:
            void createElements();
            void repositionElements();
            void restyleElements();
            SamIot::Graphics::Point getReferencePoint();
            void setPartSizes();
            virtual void addToRenderer();
    };
}

#endif