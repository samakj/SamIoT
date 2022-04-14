#include "Element.h"

SamIoT::Graphics::Elements::Element::Element(
    TFT_eSPI *_tft,
    int16_t _x,
    int16_t _y,
    int16_t _height,
    int16_t _width,
    uint16_t _zIndex,
    bool isManaged) : tft(_tft),
                      x(_x),
                      y(_y),
                      height(_height),
                      width(_width),
                      zIndex(_zIndex)
{
    if (!isManaged)
        addToRenderer();
}

void SamIoT::Graphics::Elements::Element::setX(int16_t _x)
{
    x = _x;
};
void SamIoT::Graphics::Elements::Element::setY(int16_t _y)
{
    y = _y;
};
void SamIoT::Graphics::Elements::Element::setHeight(int16_t _height)
{
    height = _height;
};
void SamIoT::Graphics::Elements::Element::setWidth(int16_t _width)
{
    width = _width;
};
void SamIoT::Graphics::Elements::Element::setVisible(bool _visible)
{
    visible = _visible;
};

SamIoT::Graphics::Point SamIoT::Graphics::Elements::Element::getDimensions()
{
    SamIoT::Graphics::Point dimensions = {height, width};
    return dimensions;
};

SamIoT::Graphics::Point SamIoT::Graphics::Elements::Element::getDisplayedDimensions()
{
    SamIoT::Graphics::Point dimensions = {displayedHeight, displayedWidth};
    return dimensions;
};

SamIoT::Graphics::Box SamIoT::Graphics::Elements::Element::getBounds()
{
    int16_t endX = x + width;
    int16_t endY = y + height;
    SamIoT::Graphics::Box bounds = {
        {x, y},
        {endX, endY}};
    return bounds;
};

SamIoT::Graphics::Box SamIoT::Graphics::Elements::Element::getDisplayedBounds()
{
    int16_t endX = displayedX + displayedWidth;
    int16_t endY = displayedY + displayedHeight;
    SamIoT::Graphics::Box bounds = {
        {displayedX, displayedY},
        {endX, endY}};
    return bounds;
};

void SamIoT::Graphics::Elements::Element::loop() {}

void SamIoT::Graphics::Elements::Element::forceRedraw()
{
    redraw = true;
}

bool SamIoT::Graphics::Elements::Element::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        height != displayedHeight ||
        width != displayedWidth);
}

void SamIoT::Graphics::Elements::Element::draw()
{
    redraw = false;
}

void SamIoT::Graphics::Elements::Element::addToRenderer()
{
    SamIoT::Graphics::Renderer.addElement(this);
}