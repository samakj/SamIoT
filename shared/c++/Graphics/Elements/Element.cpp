#include "Element.h"

SamIot::Graphics::Elements::Element::Element(
    TFT_eSPI* _tft,
    int16_t _x, 
    int16_t _y, 
    int16_t _height, 
    int16_t _width,
    uint16_t _zIndex,
    bool isManaged
) : tft(_tft),
    x(_x),
    y(_y),
    height(_height),
    width(_width),
    zIndex(_zIndex)
{
    if (!isManaged) addToRenderer();
}

void SamIot::Graphics::Elements::Element::setX(int16_t _x)
{
    x = _x;
};
void SamIot::Graphics::Elements::Element::setY(int16_t _y)
{
    y = _y;
};
void SamIot::Graphics::Elements::Element::setHeight(int16_t _height)
{
    height = _height;
};
void SamIot::Graphics::Elements::Element::setWidth(int16_t _width)
{
    width = _width;
};
void SamIot::Graphics::Elements::Element::setVisible(bool _visible)
{
    visible = _visible;
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Element::getDimensions()
{
    SamIot::Graphics::Point dimensions = {height, width};
    return dimensions;
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Element::getDisplayedDimensions()
{
    SamIot::Graphics::Point dimensions = {displayedHeight, displayedWidth};
    return dimensions;
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Element::getBounds()
{
    int16_t endX = x + width;
    int16_t endY = y + height;
    SamIot::Graphics::Box bounds = {
        {x, y},
        {endX, endY}
    };
    return bounds;
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Element::getDisplayedBounds()
{
    int16_t endX = displayedX + displayedWidth;
    int16_t endY = displayedY + displayedHeight;
    SamIot::Graphics::Box bounds = {
        {displayedX, displayedY}, 
        {endX, endY}
    };
    return bounds;
};


void SamIot::Graphics::Elements::Element::loop() {}

void SamIot::Graphics::Elements::Element::forceRedraw()
{
    redraw = true;
}

bool SamIot::Graphics::Elements::Element::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        height != displayedHeight ||
        width != displayedWidth
    );
}

void SamIot::Graphics::Elements::Element::draw()
{
    redraw = false;
}

void SamIot::Graphics::Elements::Element::addToRenderer()
{
    SamIot::Graphics::Renderer.addElement(this);
}