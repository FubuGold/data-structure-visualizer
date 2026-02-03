#include "../../include/UI/GUI-scene.h"

#include <iostream>

namespace GUI
{

// Scene base class implementation

template<typename T>
requires std::derived_from<T,sf::Drawable>
void Scene::addElement(std::shared_ptr<T> element_ptr)
{
    elements.push_back(std::static_pointer_cast<sf::Drawable>(element_ptr));
    if constexpr (std::derived_from<T,IInteractableElement>)
    {
        interactableElements.push_back(std::static_pointer_cast<IInteractableElement>(element_ptr));
    }
}

void Scene::handleEvent(const std::optional<sf::Event>& e)
{
    for (std::shared_ptr<IInteractableElement> element : interactableElements)
    {
        element->handleEvent(e);
    }
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates state)
{
    for (std::shared_ptr<sf::Drawable> element : elements)
    {
        target.draw(*element,state);
    }
}

void Scene::setWindow(sf::RenderTarget *target_ptr)
{
    this->target_ptr = target_ptr;
}

//======================================================//

// Debug scene implementation

void DebugScene::setup()
{
    std::shared_ptr<RectangleButton<>> button1(new RectangleButton<>(
        {400, 100}, {0,0},
        "This is a button",
        20, 0, 2,
        sf::Color::Black, sf::Color::White, sf::Color::Blue
    ));
    addElement(button1);
    button1->setClickCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Click registered\n";
    });
    button1->setReleaseCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Release registered\n";
    });
    button1->setHoverInCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Hover in detected\n";
    });
    button1->setHoverOutCallback([](sf::RectangleShape &rect,sf::Text &text){
        std::cerr << "Hover out detected\n";
    });



    for (std::shared_ptr<IInteractableElement> element : interactableElements)
    {
        element->setWindow(target_ptr);
    }
}

}