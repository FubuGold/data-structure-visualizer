// #ifndef UI_GUI_ELEMENT_IMPLEMENTATION
// #define UI_GUI_ELEMENT_IMPLEMENTATION

#include <iostream> // Debug
#include <SFML/Graphics.hpp>
#include <functional>
#include <concepts>
#include <assert.h>

// #include "GUI-interactable-element.h"
#include "../global/config.h"

namespace GUI
{

//======================================================//

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
    if (hovered) return;
    hovered = true;
    onHoverIn();
}
template<typename callback_t, typename ...Args>
requires std::invocable<callback_t, Args...>
void InteractableElement<callback_t,Args...>::hoverOut()
{
    if (!hovered) return;
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

}

// #endif