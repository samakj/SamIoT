#include "Text.h"


SamIot::Graphics::Elements::Text::Text(
    TFT_eSPI* tft,
    int16_t x, 
    int16_t y,
    std::string _text,
    const GFXfont* _font,
    int16_t _color,
    int16_t _background,
    uint8_t _datum,
    uint16_t _zIndex,
    bool isManaged
) : SamIot::Graphics::Elements::Element(tft, x, y, 0, 0, _zIndex, true),
    text(_text),
    font(_font),
    color(_color),
    background(_background),
    datum(_datum)
{
    if (!isManaged) addToRenderer();
}

void SamIot::Graphics::Elements::Text::setText(std::string _text)
{
    text = _text;
};
void SamIot::Graphics::Elements::Text::setFont(const GFXfont* _font)
{
    font = _font;
};
void SamIot::Graphics::Elements::Text::setColor(int16_t _color)
{
    color = _color;
};
void SamIot::Graphics::Elements::Text::setBackground(int16_t _background)
{
    background = _background;
};
void SamIot::Graphics::Elements::Text::setDatum(uint8_t _datum)
{
    datum = _datum;
};
void SamIot::Graphics::Elements::Text::setVisible(bool _visible)
{
    if (visible && !_visible) 
    {
        SamIot::Graphics::Box bounds = getDisplayedBounds();
        tft->fillRect(
            bounds.topLeft.x,
            bounds.topLeft.y,
            bounds.bottomRight.x,
            bounds.bottomRight.y,
            displayedBackground
        );
    }
    visible = _visible;
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Text::getDimensions()
{
    SamIot::Graphics::Point dimensions = {0, 0};

    tft->setFreeFont(font);
    dimensions.y = (int16_t)tft->fontHeight();
    dimensions.x = (int16_t)tft->textWidth(text.c_str());

    return dimensions;
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Text::getDisplayedDimensions()
{
    SamIot::Graphics::Point dimensions = {0, 0};

    tft->setFreeFont(displayedFont);
    dimensions.y = (int16_t)tft->fontHeight();
    dimensions.x = (int16_t)tft->textWidth(displayedText.c_str());

    return dimensions;
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Text::getBounds()
{
    SamIot::Graphics::Box bounds = {{0, 0}, {0, 0}};

    SamIot::Graphics::Point dimensions = getDimensions();

    if (datum == TL_DATUM || datum == ML_DATUM || datum == BL_DATUM)
    {
        bounds.topLeft.x = x;
        bounds.bottomRight.x = x + dimensions.x;
    }
    if (datum == TC_DATUM || datum == MC_DATUM || datum == BC_DATUM)
    {
        bounds.topLeft.x = x - std::ceil(dimensions.x / 2.);
        bounds.bottomRight.x = x + std::ceil(dimensions.x / 2.);
    }
    if (datum == TR_DATUM || datum == MR_DATUM || datum == BR_DATUM)
    {
        bounds.topLeft.x = x - dimensions.x;
        bounds.bottomRight.x = x;
    }
    if (datum == TL_DATUM || datum == TC_DATUM || datum == TR_DATUM)
    {
        bounds.topLeft.y = y;
        bounds.bottomRight.y = y + dimensions.y;
    }
    if (datum == ML_DATUM || datum == MC_DATUM || datum == MR_DATUM)
    {
        bounds.topLeft.y = y - std::ceil(dimensions.y / 2.);
        bounds.bottomRight.y = y + std::ceil(dimensions.y / 2.);
    }
    if (datum == BL_DATUM || datum == BC_DATUM || datum == BR_DATUM)
    {
        bounds.topLeft.y = y - dimensions.y;
        bounds.bottomRight.y = y;
    }

    return bounds;
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Text::getDisplayedBounds()
{
    SamIot::Graphics::Box bounds = {{0, 0}, {0, 0}};

    SamIot::Graphics::Point dimensions = getDisplayedDimensions();

    if (displayedDatum == TL_DATUM || displayedDatum == ML_DATUM || displayedDatum == BL_DATUM)
    {
        bounds.topLeft.x = displayedX;
        bounds.bottomRight.x = displayedX + dimensions.x;
    }
    if (displayedDatum == TC_DATUM || displayedDatum == MC_DATUM || displayedDatum == BC_DATUM)
    {
        bounds.topLeft.x = displayedX - std::ceil(dimensions.x / 2.);
        bounds.bottomRight.x = displayedX + std::ceil(dimensions.x / 2.);
    }
    if (displayedDatum == TR_DATUM || displayedDatum == MR_DATUM || displayedDatum == BR_DATUM)
    {
        bounds.topLeft.x = displayedX - dimensions.x;
        bounds.bottomRight.x = displayedX;
    }
    if (displayedDatum == TL_DATUM || displayedDatum == TC_DATUM || displayedDatum == TR_DATUM)
    {
        bounds.topLeft.y = displayedY;
        bounds.bottomRight.y = displayedY + dimensions.y;
    }
    if (displayedDatum == ML_DATUM || displayedDatum == MC_DATUM || displayedDatum == MR_DATUM)
    {
        bounds.topLeft.y = displayedY - std::ceil(dimensions.y / 2.);
        bounds.bottomRight.y = displayedY + std::ceil(dimensions.y / 2.);
    }
    if (displayedDatum == BL_DATUM || displayedDatum == BC_DATUM || displayedDatum == BR_DATUM)
    {
        bounds.topLeft.y = displayedY - dimensions.y;
        bounds.bottomRight.y = displayedY;
    }

    return bounds;
};

bool SamIot::Graphics::Elements::Text::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        text != displayedText ||
        font != displayedFont ||
        color != displayedColor ||
        background != displayedBackground ||
        datum != displayedDatum
    );
};

void SamIot::Graphics::Elements::Text::configureTFTFontStyles()
{
    tft->setFreeFont(font);
    tft->setTextDatum(datum);
    tft->setTextColor(color, background);
};

void SamIot::Graphics::Elements::Text::draw()
{
    if (height && width)
    {
        SamIot::Graphics::Box oldBounds = getDisplayedBounds();
        SamIot::Graphics::Box newBounds = getBounds();
        std::vector<Box> fillBoxs = uncontainedBoxs(oldBounds, newBounds);

        for (SamIot::Graphics::Box &box : fillBoxs)
        {
            tft->fillRect(
                box.topLeft.x - 1,
                box.topLeft.y - 1,
                box.bottomRight.x - box.topLeft.x + 1,
                box.bottomRight.y - box.topLeft.y + 1,
                background
            );
        }
    }

    configureTFTFontStyles();
    tft->drawString(text.c_str(), x, y);

    SamIot::Graphics::Point newDimensions = getDimensions();

    height = newDimensions.y;
    width = newDimensions.x;
    displayedX = x;
    displayedY = y;
    displayedHeight = height;
    displayedWidth = width;
    displayedText = text;
    displayedFont = font;
    displayedColor = color;
    displayedBackground = background;
    displayedDatum = datum;
    redraw = false;
};

void SamIot::Graphics::Elements::Text::addToRenderer()
{
    SamIot::Graphics::Renderer.addElement(this);
}