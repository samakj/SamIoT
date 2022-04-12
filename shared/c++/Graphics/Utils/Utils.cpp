#include "Utils.h"

bool SamIoT::Graphics::boxContainsPoint(
    SamIoT::Graphics::Box box, 
    SamIoT::Graphics::Point point)
{
    return (
        box.topLeft.x <= point.x &&
        box.topLeft.y <= point.y &&
        box.bottomRight.x >= point.x &&
        box.bottomRight.y >= point.y
    );
}

bool SamIoT::Graphics::boxContainsBox(
    SamIoT::Graphics::Box box, 
    SamIoT::Graphics::Box box2)
{
    return (
        SamIoT::Graphics::boxContainsPoint(box, box2.topLeft) &&
        SamIoT::Graphics::boxContainsPoint(box, box2.bottomRight)
    );
}

bool SamIoT::Graphics::boxIntersectsBox(
    SamIoT::Graphics::Box box, 
    SamIoT::Graphics::Box box2)
{
    return (
        (box.topLeft.y > box2.topLeft.y && box.topLeft.y < box2.bottomRight.y && box.topLeft.y < box2.bottomRight.y && box.bottomRight.y > box2.topLeft.y) ||
        (box.bottomRight.y > box2.topLeft.y && box.bottomRight.y < box2.bottomRight.y && box.topLeft.y < box2.bottomRight.y && box.bottomRight.y > box2.topLeft.y) ||
        (box.topLeft.x > box2.topLeft.x && box.topLeft.x < box2.bottomRight.x && box.topLeft.x < box2.bottomRight.x && box.bottomRight.x > box2.topLeft.x) ||
        (box.bottomRight.x > box2.topLeft.x && box.bottomRight.x < box2.bottomRight.x && box.topLeft.x < box2.bottomRight.x && box.bottomRight.x > box2.topLeft.x)
    );
}

std::vector<SamIoT::Graphics::Box> SamIoT::Graphics::uncontainedBoxs(
    SamIoT::Graphics::Box oldBox, 
    SamIoT::Graphics::Box newBox)
{
    std::vector<SamIoT::Graphics::Box> boxs = {};

    if (SamIoT::Graphics::boxContainsBox(newBox, oldBox)) return boxs;

    if (SamIoT::Graphics::boxIntersectsBox(newBox, oldBox)) {
        if (oldBox.topLeft.x < newBox.topLeft.x) {
            SamIoT::Graphics::Box box;
            SamIoT::Graphics::Point p1, p2;
            p1.x = oldBox.topLeft.x;
            p1.y = oldBox.topLeft.y;
            p2.x = newBox.topLeft.x;
            p2.y = oldBox.bottomRight.y;
            box.topLeft = p1;
            box.bottomRight = p2;
            boxs.push_back(box);
        }
        if (oldBox.bottomRight.x > newBox.bottomRight.x) {
            SamIoT::Graphics::Box box;
            SamIoT::Graphics::Point p1, p2;
            p1.x = newBox.bottomRight.x;
            p1.y = oldBox.topLeft.y;
            p2.x = oldBox.bottomRight.x;
            p2.y = oldBox.bottomRight.y;
            box.topLeft = p1;
            box.bottomRight = p2;
            boxs.push_back(box);
        }
        if (oldBox.topLeft.y < newBox.topLeft.y) {
            SamIoT::Graphics::Box box;
            SamIoT::Graphics::Point p1, p2;
            p1.x = newBox.topLeft.x;
            p1.y = oldBox.topLeft.y;
            p2.x = newBox.bottomRight.x;
            p2.y = newBox.topLeft.y;
            box.topLeft = p1;
            box.bottomRight = p2;
            boxs.push_back(box);
        }
        if (oldBox.bottomRight.y > newBox.bottomRight.y) {
            SamIoT::Graphics::Box box;
            SamIoT::Graphics::Point p1, p2;
            p1.x = newBox.topLeft.x;
            p1.y = newBox.bottomRight.y;
            p2.x = newBox.bottomRight.x;
            p2.y = oldBox.bottomRight.y;
            box.topLeft = p1;
            box.bottomRight = p2;
            boxs.push_back(box);
        }
    } else {
        boxs.push_back(oldBox);
    }

    return boxs;
}