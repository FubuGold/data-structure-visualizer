#include "../../include/UI/GUI-component.h"

namespace GUI
{
    // Base element class implementation

    // Helper functions

    void BaseElement::onClick()
    {
        if (onClickCallback) onClickCallback();
    }
    void BaseElement::onHoverIn()
    {
        if (onHoverInCallback) onHoverInCallback();
    }
    void BaseElement::onHoverOut()
    {
        if (onHoverOutCallback) onHoverOutCallback();
    }
    void BaseElement::onRelease()
    {
        if (onReleaseCallback) onReleaseCallback();
    }

    bool BaseElement::containPos(sf::Vector2f pos)
    {
        return interactBound.getGlobalBounds().contains(pos);
    }

    void BaseElement::click(sf::Vector2f pos)
    {
        if (clicked) return;
        if (!containPos(pos)) return;
        clicked = true;
        onClick();
    }
    void BaseElement::release(sf::Vector2f pos)
    {
        if (!clicked) return;
        if (containPos(pos)) onRelease();
        clicked = false;
    }
    void BaseElement::hoverIn(sf::Vector2f pos)
    {
        if (hovered) return;
        if (!containPos(pos)) return;
        hovered = true;
        onHoverIn();
    }
    void BaseElement::hoverOut(sf::Vector2f pos)
    {
        if (!hovered) return;
        if (containPos(pos)) return;
        onHoverOut();
        hovered = false;
    }

    // Setter functions
    
    void BaseElement::setClickCallback(callback_t callback)
    {
        onClickCallback = callback;
    }
    void BaseElement::setHoverInCallback(callback_t callback)
    {
        onHoverInCallback = callback;
    }
    void BaseElement::setHoverOutCallback(callback_t callback)
    {
        onHoverOutCallback = callback;
    }
    void BaseElement::setReleaseCallback(callback_t callback)
    {
        onReleaseCallback = callback;
    }

    void BaseElement::setWindow(std::shared_ptr<sf::RenderWindow> window_ptr)
    {
        window = window_ptr;
    }

    void BaseElement::pollEvent(const sf::Event& e) {}

    //---------------------------------------------
}