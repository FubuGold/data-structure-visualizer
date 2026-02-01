#include "../../include/UI/GUI-scene.h"

namespace GUI
{

// Scene base class implementation

template<typename T>
requires std::derived_from<T,BaseElement>
void Scene::addElement(std::shared_ptr<T> element_ptr)
{
    elements.push_back(std::static_pointer_cast<BaseElement>(element_ptr));
    if constexpr (std::derived_from<T,IInteractiveElement>)
    {
        interactableElements.push_back(std::static_pointer_cast<IInteractiveElement>(element_ptr));
    }
}

void Scene::setupWindow(sf::RenderWindow *window_ptr)
{
    window = window_ptr;
}

void Scene::draw()
{
    for (std::shared_ptr<BaseElement> element : elements)
    {
        element->draw();
    }
}

//======================================================//

// Debug scene implementation

void DebugScene::setup()
{
    
}

}