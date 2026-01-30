#include "../../include/UI/GUI-component.h"
#include <iostream> // Debug

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

template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::onClick()
{
    if (onClickCallback) onClickCallback();
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::onHoverIn()
{
    if (onHoverInCallback) onHoverInCallback();
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::onHoverOut()
{
    if (onHoverOutCallback) onHoverOutCallback();
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::onRelease()
{
    if (onReleaseCallback) onReleaseCallback();
}

template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::click(sf::Vector2f pos)
{
    if (clicked || !hovered) return;
    clicked = true;
    onClick();
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::release(sf::Vector2f pos)
{
    if (!clicked) return;
    if (hovered) onRelease();
    clicked = false;
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::hoverIn(sf::Vector2f pos)
{
    if (hovered) return;
    if (!containPos(pos)) return;
    hovered = true;
    onHoverIn();
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::hoverOut(sf::Vector2f pos)
{
    if (!hovered) return;
    if (containPos(pos)) return;
    if (!clicked) onHoverOut();
    hovered = false;
}

// Public functions

template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::setClickCallback(callback_t callback)
{
    onClickCallback = callback;
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::setHoverInCallback(callback_t callback)
{
    onHoverInCallback = callback;
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::setHoverOutCallback(callback_t callback)
{
    onHoverOutCallback = callback;
}
template<typename callback_t>
requires std::invocable<callback_t> 
        && std::same_as<std::invoke_result_t<callback_t>, void>
void BaseElement<callback_t>::setReleaseCallback(callback_t callback)
{
    onReleaseCallback = callback;
}

//======================================================//

}