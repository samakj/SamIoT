#ifndef _SamIoT_Graphics_Elements_Metric_h
#define _SamIoT_Graphics_Elements_Metric_h

#include <TFT_eSPI.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "Text.h"

namespace SamIoT::Graphics::Elements
{
    class Metric : public SamIoT::Graphics::Elements::Element
    {
    public:
        std::string value = "...";
        std::string unit = "";
        const GFXfont *font = nullptr;
        int16_t color = TFT_WHITE;
        int16_t background = TFT_BLACK;
        uint8_t datum = TL_DATUM;

    protected:
        std::string displayedValue = "...";
        std::string displayedUnit = "";
        const GFXfont *displayedFont = nullptr;
        int16_t displayedColor = TFT_WHITE;
        int16_t displayedBackground = TFT_BLACK;
        uint8_t displayedDatum = TL_DATUM;
        int16_t unitPadding = 0;

        std::vector<SamIoT::Graphics::Elements::Text> elements = {};

    public:
        Metric(
            TFT_eSPI *tft = nullptr,
            int16_t x = 0,
            int16_t y = 0,
            std::string value = "...",
            std::string unit = "",
            const GFXfont *font = nullptr,
            int16_t color = TFT_WHITE,
            int16_t background = TFT_BLACK,
            uint8_t datum = TL_DATUM);

        void setX(int16_t x);
        void setY(int16_t y);
        void setValue(std::string value = "...");
        void setUnit(std::string unit = "");
        void setFont(const GFXfont *font = nullptr);
        void setColor(int16_t color = TFT_WHITE);
        void setBackground(int16_t background = TFT_BLACK);
        void setDatum(uint8_t datum = TL_DATUM);

        SamIoT::Graphics::Point getDimensions();
        SamIoT::Graphics::Point getDisplayedDimensions();
        SamIoT::Graphics::Box getBounds();
        SamIoT::Graphics::Box getDisplayedBounds();

        void configureTFTFontStyles();

        void loop();

        bool needsRedraw();
        void draw();

    private:
        void createElements();
        void repositionElements();
        void restyleElements();
        SamIoT::Graphics::Point getReferencePoint();
    };
}

#endif