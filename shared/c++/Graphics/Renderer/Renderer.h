#ifndef _SamIoT_Graphics_Renderer_h
#define _SamIoT_Graphics_Renderer_h

#include <vector>

#include "../Elements/Element.h"

namespace SamIoT::Graphics
{
    namespace Elements
    {
        class Element;
    }

    class RendererClass
    {
    public:
        std::vector<SamIoT::Graphics::Elements::Element *> elements;

    private:
        uint16_t drawIndex = 0;

    public:
        RendererClass(const RendererClass &) = delete;
        static RendererClass &getInstance();

        void addElement(SamIoT::Graphics::Elements::Element *element);

        void draw();

    private:
        RendererClass(){};
        static RendererClass _instance;
    };

    static RendererClass &Renderer = RendererClass::getInstance();
}

#endif