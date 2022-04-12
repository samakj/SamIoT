#include "Metric.h"

SamIot::Graphics::Elements::Metric::Metric(
    TFT_eSPI* tft,
    int16_t x, 
    int16_t y,
    std::string _value,
    std::string _unit,
    const GFXfont* _font,
    int16_t _color,
    int16_t _background,
    uint8_t _datum
) : Element(tft, x, y, 0, 0),
    value(_value),
    unit(_unit),
    font(_font),
    color(_color),
    background(_background),
    datum(_datum)
{}

void SamIot::Graphics::Elements::Metric::setX(int16_t _x)
{
    x = _x;
    repositionElements();
};
void SamIot::Graphics::Elements::Metric::setY(int16_t _y)
{
    y = _y;
    repositionElements();
};
void SamIot::Graphics::Elements::Metric::setValue(std::string _value)
{
    value = _value;
    if (elements.size()) elements[0].setText(value);
    repositionElements();
};
void SamIot::Graphics::Elements::Metric::setUnit(std::string _unit)
{
    unit = _unit;
    if (elements.size()) elements[1].setText(unit);
    repositionElements();
};
void SamIot::Graphics::Elements::Metric::setFont(const GFXfont* _font)
{
    font = _font;
    repositionElements();
    restyleElements();
};
void SamIot::Graphics::Elements::Metric::setColor(int16_t _color)
{
    color = _color;
    restyleElements();
};
void SamIot::Graphics::Elements::Metric::setBackground(int16_t _background)
{
    background = _background;
    restyleElements();
};
void SamIot::Graphics::Elements::Metric::setDatum(uint8_t _datum)
{
    datum = _datum;
    repositionElements();
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Metric::getDimensions()
{
    configureTFTFontStyles();

    int16_t _x = unitPadding;
    _x += (int16_t)tft->textWidth(value.c_str());
    _x += (int16_t)tft->textWidth(unit.c_str()); 

    return {
        _x,
        (int16_t)tft->fontHeight()
    };
};

SamIot::Graphics::Point SamIot::Graphics::Elements::Metric::getDisplayedDimensions()
{
    configureTFTFontStyles();

    int16_t _x = unitPadding;
    _x += (int16_t)tft->textWidth(displayedValue.c_str());
    _x += (int16_t)tft->textWidth(displayedUnit.c_str()); 
    return {
        _x,
        (int16_t)tft->fontHeight()
    };
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Metric::getBounds()
{
    SamIot::Graphics::Box bounds = {{10000, 10000}, {0, 0}};

    for (SamIot::Graphics::Elements::Text &element : elements)
    {
        SamIot::Graphics::Box _bounds = element.getBounds();
        if (_bounds.topLeft.x < bounds.topLeft.x) bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.topLeft.y < bounds.topLeft.y) bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.bottomRight.x > bounds.bottomRight.x) bounds.bottomRight.x = _bounds.bottomRight.x;
        if (_bounds.bottomRight.y > bounds.bottomRight.y) bounds.bottomRight.x = _bounds.bottomRight.x;
    }
    return bounds;
};

SamIot::Graphics::Box SamIot::Graphics::Elements::Metric::getDisplayedBounds()
{
    SamIot::Graphics::Box bounds = {{10000, 10000}, {0, 0}};

    for (SamIot::Graphics::Elements::Text &element : elements)
    {
        SamIot::Graphics::Box _bounds = element.getDisplayedBounds();
        if (_bounds.topLeft.x < bounds.topLeft.x) bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.topLeft.y < bounds.topLeft.y) bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.bottomRight.x > bounds.bottomRight.x) bounds.bottomRight.x = _bounds.bottomRight.x;
        if (_bounds.bottomRight.y > bounds.bottomRight.y) bounds.bottomRight.x = _bounds.bottomRight.x;
    }
    return bounds;
};

void SamIot::Graphics::Elements::Metric::configureTFTFontStyles()
{
    tft->setFreeFont(font);
    tft->setTextDatum(TL_DATUM);
    tft->setTextColor(color, background);
};

void SamIot::Graphics::Elements::Metric::loop()
{
    if (needsRedraw()) draw();
}

bool SamIot::Graphics::Elements::Metric::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        value != displayedValue ||
        unit != displayedUnit ||
        font != displayedFont ||
        color != displayedColor ||
        background != displayedBackground ||
        datum != displayedDatum
    );
};

void SamIot::Graphics::Elements::Metric::draw()
{
    if (!elements.size()) createElements();
    if (height && width)
    {
        SamIot::Graphics::Box oldBounds = getDisplayedBounds();
        SamIot::Graphics::Box newBounds = getBounds();
        std::vector<SamIot::Graphics::Box> fillBoxs = uncontainedBoxs(oldBounds, newBounds);

        for (SamIot::Graphics::Box &box : fillBoxs)
        {
            tft->fillRect(
                box.topLeft.x,
                box.topLeft.y,
                box.bottomRight.x - box.topLeft.x,
                box.bottomRight.y - box.topLeft.y,
                background
            );
        }
    }
    for (SamIot::Graphics::Text &element : elements)
    {
        if (element.needsRedraw()) element.draw();
    }

    SamIot::Graphics::Point newDimensions = getDimensions();

    height = newDimensions.y;
    width = newDimensions.x;
    displayedX = x;
    displayedY = y;
    displayedHeight = height;
    displayedWidth = width;
    displayedValue = value;
    displayedUnit = unit;
    displayedFont = font;
    displayedColor = color;
    displayedBackground = background;
    displayedDatum = datum;
    redraw = false;
};

void SamIot::Graphics::Elements::Metric::createElements()
{
    SamIot::Graphics::Point reference = getReferencePoint();

    SamIot::Graphics::Text ValueElement(
        tft,
        reference.x,
        reference.y,
        value,
        font,
        color,
        background,
        TL_DATUM
    );

    SamIot::Graphics::Text UnitElement(
        tft,
        reference.x + ValueElement.getDimensions().x + unitPadding,
        reference.y,
        unit,
        font,
        color,
        background,
        TL_DATUM
    );

    elements.push_back(ValueElement);
    elements.push_back(UnitElement);
}

void SamIot::Graphics::Elements::Metric::repositionElements()
{
    if (!elements.size()) return;
    SamIot::Graphics::Point reference = getReferencePoint();

    elements[0].setX(reference.x);
    elements[0].setY(reference.y);
    elements[1].setX(reference.x + elements[0].getDimensions().x + unitPadding);
    elements[1].setY(reference.y);
};

void SamIot::Graphics::Elements::Metric::restyleElements()
{
    if (!elements.size()) return;

    for (SamIot::Graphics::Elements::Text &element : elements)
    {
        element.setFont(font);
        element.setColor(color);
        element.setBackground(background);
    }
}

SamIot::Graphics::Point SamIot::Graphics::Elements::Metric::getReferencePoint()
{
    SamIot::Graphics::Point dimensions = getDimensions();
    SamIot::Graphics::Point reference = {x, y};

    if (datum == TC_DATUM || datum == MC_DATUM || datum == BC_DATUM)
    {
        reference.x = x - std::ceil(dimensions.x * 0.5);
    }
    if (datum == TR_DATUM || datum == MR_DATUM || datum == BR_DATUM)
    {
        reference.x = x - dimensions.x;
    }
    if (datum == ML_DATUM || datum == MC_DATUM || datum == MR_DATUM)
    {
        reference.y = y - std::ceil(dimensions.y * 0.5);
    }
    if (datum == BL_DATUM || datum == BC_DATUM || datum == BR_DATUM)
    {
        reference.y = y - dimensions.y;
    }

    return reference;
}