// #ifndef UI_GUI_ELEMENT_IMPLEMENTATION
// #define UI_GUI_ELEMENT_IMPLEMENTATION

#include <iostream> // Debug
#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>
#include <assert.h>

#include "GUI-interactable-element.h"
#include "../config/config.h"

namespace GUI
{

// Helper functions

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::onClick()
{    
    if constexpr (std::invocable<callback_t>) {
        if (onClickCallback) onClickCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::onHoverIn()
{
    if constexpr (std::invocable<callback_t>) {
        if (onHoverInCallback) onHoverInCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::onHoverOut()
{
    if constexpr (std::invocable<callback_t>) {
        if (onHoverOutCallback) onHoverOutCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::onRelease()
{
    if constexpr (std::invocable<callback_t>) {
        if (onReleaseCallback) onReleaseCallback();
    }
}

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::click()
{
    if (clicked || !hovered) return;
    clicked = true;
    onClick();
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::release()
{
    if (!clicked) return;
    if (hovered) onRelease();
    clicked = false;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::hoverIn()
{
    std::cerr << "Hover in called\n";
    if (hovered) return;
    std::cerr << "Hover in called\n";
    hovered = true;
    onHoverIn();
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::hoverOut()
{
    std::cerr << "Hover out called\n";
    if (!hovered) return;
    std::cerr << "Hover out called\n";
    onHoverOut();
    hovered = false;
}

// Public functions

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::setClickCallback(callback_t callback)
{
    this->onClickCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::setHoverInCallback(callback_t callback)
{
    this->onHoverInCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::setHoverOutCallback(callback_t callback)
{
    this->onHoverOutCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::setReleaseCallback(callback_t callback)
{
    this->onReleaseCallback = callback;
}

//======================================================//

// Rectangle button implementation

// Protected functions (currently for calling the callback)

void RectangleButton::onClick()
{
    if (this->onClickCallback) this->onClickCallback(rect,text);
}
void RectangleButton::onRelease()
{
    if (this->onReleaseCallback) this->onReleaseCallback(rect,text);
}
void RectangleButton::onHoverIn()
{
    if (this->onHoverInCallback) this->onHoverInCallback(rect,text);
}
void RectangleButton::onHoverOut()
{
    if (this->onHoverOutCallback) this->onHoverOutCallback(rect,text);
}

// Public functions

RectangleButton::RectangleButton(
    sf::Vector2f btnSize,
    sf::Vector2f btnPos,
    std::string text,
    int characterSize,
    int textOutline,
    int borderThickness,
    sf::Color borderColor,
    sf::Color bgColor,
    sf::Color textColor
  ) : text(Config::font)
{
    this->rect.setSize(btnSize);
    
    this->rect.setOrigin(btnSize * 0.5f); 

    this->rect.setPosition(btnPos + btnSize * 0.5f);
    this->rect.setFillColor(bgColor);
    this->rect.setOutlineThickness(borderThickness);
    this->rect.setOutlineColor(borderColor);

    this->text.setString(text);
    this->text.setOutlineThickness(textOutline);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(textColor);

    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition(btnPos + btnSize * 0.5f);
    this->text.setPosition(btnPos);
}

void RectangleButton::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->rect,state);
    target.draw(this->text,state);
}

bool RectangleButton::containPos(sf::Vector2f pos)
{
    return rect.getGlobalBounds().contains(pos);
}

void RectangleButton::handleEvent(const std::optional<sf::Event>& e)
{
    if (const sf::Event::MouseMoved *mouse_moved = e->getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mouse_pos = this->target_ptr->mapPixelToCoords(mouse_moved->position);
        if (containPos(mouse_pos)) this->hoverIn();
        else this->hoverOut();
    }
    else if (const sf::Event::MouseButtonPressed *mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mouse_pos = this->target_ptr->mapPixelToCoords(mouse_pressed->position);
        if (mouse_pressed->button == sf::Mouse::Button::Left && containPos(mouse_pos)) this->click();
    }
    else if (const sf::Event::MouseButtonReleased *mouse_released = e->getIf<sf::Event::MouseButtonReleased>())
    {
        sf::Vector2f mouse_pos = this->target_ptr->mapPixelToCoords(mouse_released->position);
        if (mouse_released->button == sf::Mouse::Button::Left && containPos(mouse_pos)) this->release();
    }
}

}

// #endif