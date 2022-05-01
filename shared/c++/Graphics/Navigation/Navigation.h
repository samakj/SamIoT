#ifndef _SamIoT_Navigation_Renderer_h
#define _SamIoT_Navigation_Renderer_h

#include <string>
#include <unordered_map>
#include <vector>

#include "../Elements/Element.h"
#include "../Renderer/Renderer.h"
#include <Logger.h>

namespace SamIoT::Graphics
{
    class Navigation
    {
    public:
        std::vector<SamIoT::Graphics::Elements::Element *> EMPTY_PAGE = {};
        std::string currentPage = "";
        std::unordered_map<std::string, std::vector<SamIoT::Graphics::Elements::Element *> *> pages = {};

    protected:
    public:
        Navigation();
        void addPage(std::string key, std::vector<SamIoT::Graphics::Elements::Element *> *elements);
        void addElementToPage(std::string key, SamIoT::Graphics::Elements::Element *element);
        std::vector<SamIoT::Graphics::Elements::Element *> *getPageElements(std::string key);

        void clearPage(std::string key = "");
        void goToPage(std::string key);

    protected:
    };
}

#endif