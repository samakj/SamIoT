#include "Navigation.h"

SamIoT::Graphics::Navigation::Navigation() {}

void SamIoT::Graphics::Navigation::addPage(std::string key, std::vector<SamIoT::Graphics::Elements::Element*> elements)
{
    pages[key] = elements;
}

void SamIoT::Graphics::Navigation::addElementToPage(std::string key, SamIoT::Graphics::Elements::Element* element)
{
    if (pages.find(key) == pages.end()) pages[key] = {};
    pages[key].push_back(element);
}

std::vector<SamIoT::Graphics::Elements::Element*> SamIoT::Graphics::Navigation::getPageElements(std::string key)
{
    if (pages.find(key) == pages.end()) return {};
    return pages[key];
}

void SamIoT::Graphics::Navigation::clearPage(std::string key)
{
    if (!key.size()) key = currentPage;
    if (!key.size()) return;
    for (Element* element : getPageElements(key))
        element->setVisible(false);
}

void SamIoT::Graphics::Navigation::goToPage(std::string key)
{
    if (pages.find(key) == pages.end()) return;
    clearPage();
    for (Element* element : pages[key])
        element->setVisible(true);
    
    currentPage = key;
}