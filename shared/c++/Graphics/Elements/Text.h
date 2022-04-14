#ifndef _SamIoT_Graphics_Elements_Text_h
#define _SamIoT_Graphics_Elements_Text_h

#include <TFT_eSPI.h>
#include <stdint.h>
#include <string>
#include <vector>

#include "Element.h"

namespace SamIoT::Graphics
{
    class Point;
    class Box;

    namespace Elements
    {
        class Text : public SamIoT::Graphics::Elements::Element
        {
        public:
            std::string text = "";
            const GFXfont *font = nullptr;
            int16_t color = TFT_WHITE;
            int16_t background = TFT_BLACK;
            uint8_t datum = TL_DATUM;

        protected:
            std::string displayedText = "";
            const GFXfont *displayedFont = nullptr;
            int16_t displayedColor = TFT_WHITE;
            int16_t displayedBackground = TFT_BLACK;
            uint8_t displayedDatum = TL_DATUM;

        public:
            Text(
                TFT_eSPI *tft = nullptr,
                int16_t x = 0,
                int16_t y = 0,
                std::string text = "",
                const GFXfont *font = nullptr,
                int16_t color = TFT_WHITE,
                int16_t background = TFT_BLACK,
                uint8_t datum = TL_DATUM,
                uint16_t zIndex = 1,
                bool isManaged = false);

            void setText(std::string text = "");
            void setFont(const GFXfont *font = nullptr);
            void setColor(int16_t color = TFT_WHITE);
            void setBackground(int16_t background = TFT_BLACK);
            void setDatum(uint8_t datum = TL_DATUM);
            void setVisible(bool visible = true);

            SamIoT::Graphics::Point getDimensions();
            SamIoT::Graphics::Point getDisplayedDimensions();
            SamIoT::Graphics::Box getBounds();
            SamIoT::Graphics::Box getDisplayedBounds();

            void configureTFTFontStyles();

            bool needsRedraw();
            void draw();

        protected:
            virtual void addToRenderer();
        };
    }
}

#endif