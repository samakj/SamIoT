#ifndef _SamIoT_Graphics_Elements_Arc_h
#define _SamIoT_Graphics_Elements_Arc_h

#include <vector>

#include "Element.h"

namespace SamIot::Graphics::Elements {
    class Arc : public SamIot::Graphics::Elements::Element
    {
        public:
            uint16_t radius = 0;
            int16_t startAngle = 0;
            int16_t endAngle = 0;
            int16_t color = TFT_WHITE;
            int16_t backgroundColor = TFT_BLACK;
            int8_t thickness = 1;
            int8_t segments = 1;

        protected:
            uint16_t displayedRadius = 0;
            int16_t displayedStartAngle = 0;
            int16_t displayedEndAngle = 0;
            int16_t displayedColor = TFT_WHITE;
            int16_t displayedBackgroundColor = TFT_BLACK;
            int8_t displayedThickness = 1;
            int8_t displayedSegments = 1;

        public:
            Arc(
                TFT_eSPI* tft = nullptr,
                int16_t x = 0, 
                int16_t y = 0,
                uint16_t radius = 0,
                int16_t startAngle = 0,
                int16_t endAngle = 0,
                int16_t color = TFT_WHITE,
                int16_t backgroundColor = TFT_BLACK,
                int8_t thickness = 1,
                int8_t segments = 1,
                uint16_t zIndex = 1,
                bool isManaged = false
            );

            void setRadius(uint16_t radius = 0);
            void setStartAngle(int16_t startAngle = 0);
            void setEndAngle(int16_t endAngle = 0);
            void setColor(int16_t color = TFT_WHITE);
            void setThickness(int8_t thickness = 0);
            void setSegments(int8_t segments = 1);

            std::vector<SamIoT::Graphics::Point> getPoints();
            std::vector<SamIoT::Graphics::Point> getDisplayedPoints();

            bool needsRedraw();
            bool needsFullRedraw();
            void clear();
            void drawTriangles(std::vector<SamIoT::Graphics::Point> points, int16_t color);
            void drawLines(std::vector<SamIoT::Graphics::Point> points, int16_t color);
            void draw();

        protected:
            virtual void addToRenderer();
    };
}

#endif