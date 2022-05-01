#include "Time.h"

SamIoT::Graphics::Elements::Time::Time(
    TFT_eSPI *tft,
    int16_t x,
    int16_t y,
    std::string _hours,
    std::string _minutes,
    std::string _seconds,
    const GFXfont *_font,
    int16_t _color,
    int16_t _background,
    uint8_t _datum,
    uint16_t _zIndex,
    bool isManaged) : SamIoT::Graphics::Elements::Element(tft, x, y, 0, 0, _zIndex),
                      hours(_hours),
                      minutes(_minutes),
                      seconds(_seconds),
                      font(_font),
                      color(_color),
                      background(_background),
                      datum(_datum)
{
    if (!isManaged)
        addToRenderer();
}

void SamIoT::Graphics::Elements::Time::setX(int16_t _x)
{
    x = _x;
    repositionElements();
};
void SamIoT::Graphics::Elements::Time::setY(int16_t _y)
{
    y = _y;
    repositionElements();
};
void SamIoT::Graphics::Elements::Time::setHours(std::string _hours)
{
    hours = _hours;
    if (elements.size())
        elements[0].setText(hours);
};
void SamIoT::Graphics::Elements::Time::setHours(uint8_t _hours)
{
    char __hours[8];
    sprintf(__hours, "%02d", _hours);
    setHours(__hours);
};
void SamIoT::Graphics::Elements::Time::setMinutes(std::string _minutes)
{
    minutes = _minutes;
    if (elements.size())
        elements[2].setText(minutes);
};
void SamIoT::Graphics::Elements::Time::setMinutes(uint8_t _minutes)
{
    char __minutes[8];
    sprintf(__minutes, "%02d", _minutes);
    setMinutes(__minutes);
};
void SamIoT::Graphics::Elements::Time::setSeconds(std::string _seconds)
{
    seconds = _seconds;
    if (elements.size())
        elements[4].setText(seconds);
};
void SamIoT::Graphics::Elements::Time::setSeconds(uint8_t _seconds)
{
    char __seconds[8];
    sprintf(__seconds, "%02d", _seconds);
    setSeconds(__seconds);
};
void SamIoT::Graphics::Elements::Time::setFromTime()
{
    char _hours[8], _minutes[8], _seconds[8];
    time_t tm = time(nullptr);
    strftime(_hours, sizeof(_hours), "%H", gmtime(&tm));
    strftime(_minutes, sizeof(_minutes), "%M", gmtime(&tm));
    strftime(_seconds, sizeof(_seconds), "%S", gmtime(&tm));
    setHours(_hours);
    setMinutes(_minutes);
    setSeconds(_seconds);
};
void SamIoT::Graphics::Elements::Time::setFont(const GFXfont *_font)
{
    font = _font;
    setPartSizes();
    repositionElements();
    restyleElements();
};
void SamIoT::Graphics::Elements::Time::setColor(int16_t _color)
{
    color = _color;
    restyleElements();
};
void SamIoT::Graphics::Elements::Time::setBackground(int16_t _background)
{
    background = _background;
    restyleElements();
};
void SamIoT::Graphics::Elements::Time::setDatum(uint8_t _datum)
{
    datum = _datum;
    repositionElements();
};
void SamIoT::Graphics::Elements::Time::setVisible(bool _visible)
{
    if (visible && !_visible)
    {
        SamIoT::Graphics::Box bounds = getDisplayedBounds();
        tft->fillRect(
            bounds.topLeft.x,
            bounds.topLeft.y,
            bounds.bottomRight.x,
            bounds.bottomRight.y,
            displayedBackground);
    }
    visible = _visible;
};

SamIoT::Graphics::Point SamIoT::Graphics::Elements::Time::getDimensions()
{
    if (!maxPartWidth || !colonWidth)
        setPartSizes();
    configureTFTFontStyles();
    return {
        3 * maxPartWidth +
            4 * colonPadding +
            2 * colonWidth,
        tft->fontHeight()};
};

SamIoT::Graphics::Point SamIoT::Graphics::Elements::Time::getDisplayedDimensions()
{
    if (!maxPartWidth || !colonWidth)
        setPartSizes();
    configureTFTFontStyles();
    return {
        3 * maxPartWidth +
            4 * colonPadding +
            2 * colonWidth,
        tft->fontHeight()};
};

SamIoT::Graphics::Box SamIoT::Graphics::Elements::Time::getBounds()
{
    SamIoT::Graphics::Box bounds = {{10000, 10000}, {0, 0}};

    for (SamIoT::Graphics::Elements::Text &element : elements)
    {
        SamIoT::Graphics::Box _bounds = element.getBounds();
        if (_bounds.topLeft.x < bounds.topLeft.x)
            bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.topLeft.y < bounds.topLeft.y)
            bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.bottomRight.x > bounds.bottomRight.x)
            bounds.bottomRight.x = _bounds.bottomRight.x;
        if (_bounds.bottomRight.y > bounds.bottomRight.y)
            bounds.bottomRight.x = _bounds.bottomRight.x;
    }
    return bounds;
};

SamIoT::Graphics::Box SamIoT::Graphics::Elements::Time::getDisplayedBounds()
{
    SamIoT::Graphics::Box bounds = {{10000, 10000}, {0, 0}};

    for (SamIoT::Graphics::Elements::Text &element : elements)
    {
        SamIoT::Graphics::Box _bounds = element.getDisplayedBounds();
        if (_bounds.topLeft.x < bounds.topLeft.x)
            bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.topLeft.y < bounds.topLeft.y)
            bounds.topLeft.x = _bounds.topLeft.x;
        if (_bounds.bottomRight.x > bounds.bottomRight.x)
            bounds.bottomRight.x = _bounds.bottomRight.x;
        if (_bounds.bottomRight.y > bounds.bottomRight.y)
            bounds.bottomRight.x = _bounds.bottomRight.x;
    }
    return bounds;
};

void SamIoT::Graphics::Elements::Time::configureTFTFontStyles()
{
    tft->setFreeFont(font);
    tft->setTextDatum(TC_DATUM);
    tft->setTextColor(color, background);
};

boolean SamIoT::Graphics::Elements::Time::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        hours != displayedHours ||
        minutes != displayedMinutes ||
        seconds != displayedSeconds ||
        font != displayedFont ||
        color != displayedColor ||
        background != displayedBackground ||
        datum != displayedDatum);
};

void SamIoT::Graphics::Elements::Time::draw()
{
    if (!elements.size())
        createElements();
    if (height && width)
    {
        SamIoT::Graphics::Box oldBounds = getDisplayedBounds();
        SamIoT::Graphics::Box newBounds = getBounds();
        std::vector<SamIoT::Graphics::Box> fillBoxs = uncontainedBoxs(oldBounds, newBounds);

        for (SamIoT::Graphics::Box &box : fillBoxs)
        {
            tft->fillRect(
                box.topLeft.x,
                box.topLeft.y,
                box.bottomRight.x - box.topLeft.x,
                box.bottomRight.y - box.topLeft.y,
                background);
        }
    }
    for (SamIoT::Graphics::Elements::Text &element : elements)
    {
        if (element.needsRedraw())
            element.draw();
    }

    SamIoT::Graphics::Point newDimensions = getDimensions();

    height = newDimensions.y;
    width = newDimensions.x;
    displayedX = x;
    displayedY = y;
    displayedHeight = height;
    displayedWidth = width;
    displayedHours = hours;
    displayedMinutes = minutes;
    displayedSeconds = seconds;
    displayedFont = font;
    displayedColor = color;
    displayedBackground = background;
    displayedDatum = datum;
    redraw = false;
};

void SamIoT::Graphics::Elements::Time::createElements()
{
    if (!maxPartWidth || !colonWidth)
        setPartSizes();
    SamIoT::Graphics::Point reference = getReferencePoint();

    SamIoT::Graphics::Elements::Text HoursElement(
        tft,
        reference.x + std::floor(maxPartWidth * 0.5),
        reference.y,
        hours,
        font,
        color,
        background,
        TC_DATUM,
        zIndex,
        true);
    SamIoT::Graphics::Elements::Text HoursColonElement(
        tft,
        reference.x + maxPartWidth + colonPadding + std::floor(colonWidth / 2.),
        reference.y,
        ":",
        font,
        color,
        background,
        TC_DATUM,
        zIndex,
        true);
    SamIoT::Graphics::Elements::Text MinutesElement(
        tft,
        reference.x + std::floor(maxPartWidth * 1.5) + 2 * colonPadding + colonWidth,
        reference.y,
        minutes,
        font,
        color,
        background,
        TC_DATUM,
        zIndex,
        true);
    SamIoT::Graphics::Elements::Text MinutesColonElement(
        tft,
        reference.x + 2 * maxPartWidth + 3 * colonPadding + std::floor(colonWidth * 1.5),
        reference.y,
        ":",
        font,
        color,
        background,
        TC_DATUM,
        zIndex,
        true);
    SamIoT::Graphics::Elements::Text SecondsElement(
        tft,
        reference.x + std::floor(maxPartWidth * 2.5) + 4 * colonPadding + 2 * colonWidth,
        reference.y,
        seconds,
        font,
        color,
        background,
        TC_DATUM,
        zIndex,
        true);

    elements.push_back(HoursElement);
    elements.push_back(HoursColonElement);
    elements.push_back(MinutesElement);
    elements.push_back(MinutesColonElement);
    elements.push_back(SecondsElement);
}

void SamIoT::Graphics::Elements::Time::repositionElements()
{
    if (!elements.size())
        return;
    if (!maxPartWidth || !colonWidth)
        setPartSizes();
    SamIoT::Graphics::Point reference = getReferencePoint();

    elements[0].setX(reference.x + std::floor(maxPartWidth * 0.5));
    elements[0].setY(reference.y);
    elements[1].setX(reference.x + maxPartWidth + colonPadding + std::floor(colonWidth * 0.5));
    elements[1].setY(reference.y);
    elements[2].setX(reference.x + std::floor(maxPartWidth * 1.5) + 2 * colonPadding + colonWidth);
    elements[2].setY(reference.y);
    elements[3].setX(reference.x + 2 * maxPartWidth + 3 * colonPadding + std::floor(colonWidth * 1.5));
    elements[3].setY(reference.y);
    elements[4].setX(reference.x + std::floor(maxPartWidth * 2.5) + 4 * colonPadding + 2 * colonWidth);
    elements[4].setY(reference.y);
};

void SamIoT::Graphics::Elements::Time::restyleElements()
{
    if (!elements.size())
        return;

    for (SamIoT::Graphics::Elements::Text &element : elements)
    {
        element.setFont(font);
        element.setColor(color);
        element.setBackground(background);
    }
}

SamIoT::Graphics::Point SamIoT::Graphics::Elements::Time::getReferencePoint()
{
    setPartSizes();

    SamIoT::Graphics::Point dimensions = getDimensions();
    SamIoT::Graphics::Point reference = {x, y};

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

void SamIoT::Graphics::Elements::Time::setPartSizes()
{
    tft->setFreeFont(font);
    maxPartWidth = tft->textWidth("00");
    colonWidth = tft->textWidth(":");
}

void SamIoT::Graphics::Elements::Time::addToRenderer()
{
    Renderer.addElement(this);
}