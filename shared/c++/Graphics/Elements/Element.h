#ifndef _SamIoT_Graphics_Elements_Element_h
#define _SamIoT_Graphics_Elements_Element_h

#include <stdint.h>
#include <TFT_eSPI.h>

#include "../Renderer/Renderer.h"
#include "../Utils/Utils.h"

namespace SamIoT::Graphics {
    class Rederer;
    struct Point;
    struct Line;
    struct Box;

    namespace Elements {
        class Element {
            public:
                TFT_eSPI* tft = nullptr;
                int16_t x; 
                int16_t y; 
                int16_t height; 
                int16_t width;

                bool redraw = true;
                bool visible = false;
                uint16_t zIndex = 1;

            protected:
                int16_t displayedX = 0;
                int16_t displayedY = 0;
                int16_t displayedHeight = 0;
                int16_t displayedWidth = 0;

            public:
                Element(
                    TFT_eSPI* tft = nullptr,
                    int16_t x = 0, 
                    int16_t y = 0, 
                    int16_t height = 0, 
                    int16_t width = 0,
                    uint16_t zIndex = 1,
                    bool isManaged = false
                );

                virtual void setX(int16_t x = 0);
                virtual void setY(int16_t y = 0);
                virtual void setHeight(int16_t height = 0);
                virtual void setWidth(int16_t y = 0);
                virtual void setVisible(boolean visible = true);

                virtual Point getDimensions();
                virtual Point getDisplayedDimensions();
                virtual Box getBounds();
                virtual Box getDisplayedBounds();

                virtual void loop();

                void forceRedraw();
                virtual boolean needsRedraw();
                virtual void draw();

            protected:
                virtual void addToRenderer();
        }
    }
}

#endif