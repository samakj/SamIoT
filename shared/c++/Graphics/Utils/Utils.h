#ifndef _SamIoT_Graphics_Utils_h
#define _SamIoT_Graphics_Utils_h

#include <stdint.h>
#include <vector>

namespace SamIoT::Graphics
{
    struct Point
    {
        int16_t x = 0;
        int16_t y = 0;
    };

    struct Line
    {
        Point p1;
        Point p2;
    };

    struct Box
    {
        Point topLeft;
        Point bottomRight;
    };

    bool boxContainsPoint(Box boundingBox, Point point);
    bool boxContainsBox(Box boundingBox, Box boundingBox2);
    bool boxIntersectsBox(Box boundingBox, Box boundingBox2);
    std::vector<Box> uncontainedBoxs(Box boundingBox, Box boundingBox2);
}

#endif
