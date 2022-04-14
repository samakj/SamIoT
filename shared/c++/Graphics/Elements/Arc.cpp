#include "Arc.h"

SamIoT::Graphics::Elements::Arc::Arc(
    TFT_eSPI *_tft,
    int16_t _x,
    int16_t _y,
    uint16_t _radius,
    int16_t _startAngle,
    int16_t _endAngle,
    int16_t _color,
    int16_t _backgroundColor,
    int8_t _thickness,
    int8_t _segments,
    uint16_t _zIndex,
    bool isManaged) : Element(_tft, _x, _y, _radius, _radius, _zIndex, true),
                      radius(_radius),
                      startAngle(_startAngle),
                      endAngle(_endAngle),
                      color(_color),
                      backgroundColor(_backgroundColor),
                      thickness(_thickness),
                      segments(_segments)
{
    if (!isManaged)
        addToRenderer();
};

void SamIoT::Graphics::Elements::Arc::setRadius(uint16_t _radius)
{
    radius = _radius;
};
void SamIoT::Graphics::Elements::Arc::setStartAngle(int16_t _startAngle)
{
    startAngle = _startAngle;
};
void SamIoT::Graphics::Elements::Arc::setEndAngle(int16_t _endAngle)
{
    endAngle = _endAngle;
};
void SamIoT::Graphics::Elements::Arc::setColor(int16_t _color)
{
    color = _color;
};
void SamIoT::Graphics::Elements::Arc::setThickness(int8_t _thickness)
{
    thickness = _thickness;
};
void SamIoT::Graphics::Elements::Arc::setSegments(int8_t _segments)
{
    segments = _segments;
};

std::vector<SamIoT::Graphics::Point> SamIoT::Graphics::Elements::Arc::getPoints()
{
    std::vector<SamIoT::Graphics::Point> points = {};
    float dAngle = 1. * (endAngle - startAngle) / segments;
    if (thickness > 1)
    {
        for (uint8_t i = 0; i <= segments; i++)
        {
            float angle = startAngle + i * dAngle;
            Point p1 = {0, 0};
            Point p2 = {0, 0};
            float dt = (thickness - 1) / 2.;
            p1.x = x + (radius - std::floor(dt)) * sin(angle * PI / 180);
            p1.y = y + (radius - std::floor(dt)) * -cos(angle * PI / 180);
            p2.x = x + (radius + std::ceil(dt)) * sin(angle * PI / 180);
            p2.y = y + (radius + std::ceil(dt)) * -cos(angle * PI / 180);
            if (
                !points.size() ||
                !(
                    points.back().x == p2.x &&
                    points.back().y == p2.y))
            {
                points.push_back(p1);
                points.push_back(p2);
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= segments; i++)
        {
            float angle = startAngle + i * dAngle;
            SamIoT::Graphics::Point point = {0, 0};
            point.x = x + radius * sin(angle * PI / 180);
            point.y = y + radius * -cos(angle * PI / 180);
            if (
                !points.size() ||
                !(
                    points.back().x == point.x &&
                    points.back().y == point.y))
            {
                points.push_back(point);
            }
        }
    }
    return points;
};

std::vector<SamIoT::Graphics::Point> SamIoT::Graphics::Elements::Arc::getDisplayedPoints()
{
    std::vector<SamIoT::Graphics::Point> points = {};
    float dAngle = 1. * (displayedEndAngle - displayedStartAngle) / displayedSegments;
    if (displayedThickness > 1)
    {
        for (uint8_t i = 0; i <= displayedSegments; i++)
        {
            float angle = displayedStartAngle + i * dAngle;
            Point p1 = {0, 0};
            Point p2 = {0, 0};
            float dt = (displayedThickness - 1) / 2.;
            p1.x = displayedX + (displayedRadius - std::floor(dt)) * sin(angle * PI / 180);
            p1.y = displayedY + (displayedRadius - std::floor(dt)) * -cos(angle * PI / 180);
            p2.x = displayedX + (displayedRadius + std::ceil(dt)) * sin(angle * PI / 180);
            p2.y = displayedY + (displayedRadius + std::ceil(dt)) * -cos(angle * PI / 180);
            if (
                !points.size() ||
                !(
                    points.back().x == p2.x &&
                    points.back().y == p2.y))
            {
                points.push_back(p1);
                points.push_back(p2);
            }
        }
    }
    else
    {
        for (uint8_t i = 0; i <= displayedSegments; i++)
        {
            float angle = displayedStartAngle + i * dAngle;
            SamIoT::Graphics::Point point = {0, 0};
            point.x = displayedX + displayedRadius * sin(angle * PI / 180);
            point.y = displayedY + displayedRadius * -cos(angle * PI / 180);
            if (
                !points.size() ||
                !(
                    points.back().x == point.x &&
                    points.back().y == point.y))
            {
                points.push_back(point);
            }
        }
    }
    return points;
};

bool SamIoT::Graphics::Elements::Arc::needsRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        radius != displayedRadius ||
        startAngle != displayedStartAngle ||
        endAngle != displayedEndAngle ||
        color != displayedColor ||
        backgroundColor != displayedBackgroundColor ||
        thickness != displayedThickness ||
        segments != displayedSegments);
};

bool SamIoT::Graphics::Elements::Arc::needsFullRedraw()
{
    return (
        redraw ||
        x != displayedX ||
        y != displayedY ||
        radius != displayedRadius ||
        startAngle != displayedStartAngle ||
        endAngle != displayedEndAngle ||
        thickness < displayedThickness ||
        segments != displayedSegments);
};

void SamIoT::Graphics::Elements::Arc::clear()
{
    std::vector<SamIoT::Graphics::Point> points = getDisplayedPoints();
    if (thickness > 1)
        drawTriangles(points, backgroundColor);
    else
        drawLines(points, backgroundColor);
};

void SamIoT::Graphics::Elements::Arc::drawLines(std::vector<SamIoT::Graphics::Point> points, int16_t color)
{
    for (uint8_t i = 1; i < points.size(); i++)
    {
        tft->drawLine(
            points[i - 1].x,
            points[i - 1].y,
            points[i].x,
            points[i].y,
            color);
    }
}

void SamIoT::Graphics::Elements::Arc::drawTriangles(std::vector<SamIoT::Graphics::Point> points, int16_t color)
{
    for (uint8_t i = 3; i < points.size(); i += 2)
    {
        tft->fillTriangle(
            points[i - 3].x,
            points[i - 3].y,
            points[i - 2].x,
            points[i - 2].y,
            points[i - 1].x,
            points[i - 1].y,
            color);
        tft->fillTriangle(
            points[i - 2].x,
            points[i - 2].y,
            points[i - 1].x,
            points[i - 1].y,
            points[i].x,
            points[i].y,
            color);
    }
}

void SamIoT::Graphics::Elements::Arc::draw()
{
    if (needsFullRedraw())
        clear();
    if (!radius || !thickness || startAngle == endAngle)
        return;
    std::vector<SamIoT::Graphics::Point> points = getPoints();
    if (thickness > 1)
        drawTriangles(points, color);
    else
        drawLines(points, color);

    redraw = false;
    height = radius;
    width = radius;
    displayedX = x;
    displayedY = y;
    displayedHeight = height;
    displayedWidth = width;
    displayedRadius = radius;
    displayedStartAngle = startAngle;
    displayedEndAngle = endAngle;
    displayedColor = color;
    displayedBackgroundColor = backgroundColor;
    displayedThickness = thickness;
    displayedSegments = segments;
};

void SamIoT::Graphics::Elements::Arc::addToRenderer()
{
    SamIoT::Graphics::Renderer.addElement(this);
}