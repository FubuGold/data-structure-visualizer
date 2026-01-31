#pragma once

#include <iostream> // Debug
#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>

namespace GUI
{

sf::FloatRect combineRect(sf::FloatRect rect1, sf::FloatRect rect2)
{
    const auto min = [](float a,float b) { return a < b ? a : b; };
    const auto max = [](float a,float b) { return a > b ? a : b; };

    float minX = min(rect1.position.x, rect2.position.x),
            maxX = max(rect1.position.x + rect1.size.x, rect2.position.x + rect2.size.x),
            minY = min(rect1.position.y, rect2.position.y),
            maxY = max(rect1.position.y + rect1.size.y, rect2.position.y + rect2.size.y);
    
    return sf::FloatRect({minX,minY}, {maxX - minX, maxY - minY});
}

// Base element class implementation

// Helper functions

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::onClick()
{
    if constexpr (std::invocable<callback_t>) {
        if (onClickCallback) onClickCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::onHoverIn()
{
    if constexpr (std::invocable<callback_t>) {
        if (onHoverInCallback) onHoverInCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::onHoverOut()
{
    if constexpr (std::invocable<callback_t>) {
        if (onHoverOutCallback) onHoverOutCallback();
    }
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::onRelease()
{
    if constexpr (std::invocable<callback_t>) {
        if (onReleaseCallback) onReleaseCallback();
    }
}

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::click()
{
    if (clicked || !hovered) return;
    clicked = true;
    onClick();
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::release()
{
    if (!clicked) return;
    if (hovered) onRelease();
    clicked = false;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::hoverIn()
{
    if (hovered) return;
    hovered = true;
    onHoverIn();
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::hoverOut()
{
    if (!hovered) return;
    onHoverOut();
    hovered = false;
}

// Public functions

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::setWindow(sf::RenderWindow *window_ptr)
{
    this->window = window_ptr;
}

template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::setClickCallback(callback_t callback)
{
    this->onClickCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::setHoverInCallback(callback_t callback)
{
    this->onHoverInCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::setHoverOutCallback(callback_t callback)
{
    this->onHoverOutCallback = callback;
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void BaseElement<callback_t,Args...>::setReleaseCallback(callback_t callback)
{
    this->onReleaseCallback = callback;
}

//======================================================//

// Rectangle button implementation

// Protected functions (currently for calling the callback)

template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::onClick()
{
    if (this->onClickCallback) this->onClickCallback(rect,text);
}
template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::onRelease()
{
    if (this->onReleaseCallback) this->onReleaseCallback(rect,text);
}
template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::onHoverIn()
{
    if (this->onHoverInCallback) this->onHoverInCallback(rect,text);
}
template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::onHoverOut()
{
    if (this->onHoverOutCallback) this->onHoverOutCallback(rect,text);
}

// Public functions

template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
RectangleButton<callback_t>::RectangleButton(
    sf::Vector2f btnSize,
    sf::Vector2f btnPos,
    std::string text,
    int characterSize,
    int borderThickness,
    sf::Color borderColor,
    sf::Color bgColor,
    sf::Color textColor
  ) : text(font)
{
    this->rect.setSize(btnSize);
    
    this->rect.setOrigin(btnSize * 0.5f); 

    this->rect.setPosition(btnPos + btnSize * 0.5f);
    this->rect.setFillColor(bgColor);
    this->rect.setOutlineThickness(borderThickness);
    this->rect.setOutlineColor(borderColor);

    this->text.setString(text);
    this->text.setCharacterSize(characterSize);
    this->text.setFillColor(textColor);

    this->text.setOrigin(this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition(btnPos + btnSize * 0.5f);
}

template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::draw(sf::RenderStates state)
{
    this->window->draw(rect,state);
    this->window->draw(text,state);
}

template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
bool RectangleButton<callback_t>::containPos(sf::Vector2f pos)
{
    return rect.getGlobalBounds().contains(pos);
}

template<typename callback_t>
requires std::invocable<callback_t,sf::RectangleShape&,sf::Text&>
void RectangleButton<callback_t>::handleEvent(const std::optional<sf::Event>& e)
{
    if (const sf::Event::MouseMoved *mouse_moved = e->getIf<sf::Event::MouseMoved>())
    {
        sf::Vector2f mouse_pos = this->window->mapPixelToCoords(mouse_moved->position);
        if (containPos(mouse_pos)) this->hoverIn();
        else this->hoverOut();
    }
    else if (const sf::Event::MouseButtonPressed *mouse_pressed = e->getIf<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2f mouse_pos = this->window->mapPixelToCoords(mouse_pressed->position);
        if (mouse_pressed->button == sf::Mouse::Button::Left && containPos(mouse_pos)) this->click();
    }
    else if (const sf::Event::MouseButtonReleased *mouse_released = e->getIf<sf::Event::MouseButtonReleased>())
    {
        sf::Vector2f mouse_pos = this->window->mapPixelToCoords(mouse_released->position);
        if (mouse_released->button == sf::Mouse::Button::Left && containPos(mouse_pos)) this->release();
    }
}

}