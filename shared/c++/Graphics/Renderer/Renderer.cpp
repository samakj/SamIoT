#include "Renderer.h"

SamIoT::Graphics::RendererClass SamIoT::Graphics::RendererClass::_instance;

SamIoT::Graphics::RendererClass &SamIoT::Graphics::RendererClass::getInstance()
{
    return _instance;
}

bool _compareElementZIndex(SamIoT::Graphics::Elements::Element *elementA, SamIoT::Graphics::Elements::Element *elementB)
{
    return elementA->zIndex > elementB->zIndex;
}

void SamIoT::Graphics::RendererClass::addElement(SamIoT::Graphics::Elements::Element *element)
{
    elements.push_back(element);
    std::sort(elements.begin(), elements.end(), _compareElementZIndex);
}

void SamIoT::Graphics::RendererClass::draw()
{
    uint16_t noElements = elements.size();
    if (noElements)
    {
        if (drawIndex < noElements)
        {
            uint16_t attempts = 0;
            while (attempts < noElements)
            {
                SamIoT::Graphics::Elements::Element *element = elements[drawIndex];
                if (element->visible && element->needsRedraw())
                {
                    element->draw();
                    break;
                }
                drawIndex = (drawIndex + 1) % elements.size();
                attempts++;
            }
        }
        else
            drawIndex = 0;
    }
    for (SamIoT::Graphics::Elements::Element *element : elements)
        element->loop();
}