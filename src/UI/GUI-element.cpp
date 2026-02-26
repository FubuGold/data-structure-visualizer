#include "../../include/UI/GUI-element.h"

#include "../../include/global/config.h"
#include <string>

namespace GUI
{

// Debug Dot implementation

DebugDot::DebugDot(sf::Vector2f pos)
{
    dot.setPosition(pos);
    dot.setRadius(2);
    dot.setFillColor(sf::Color::Red);
}

void DebugDot::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(dot);
}

// Bindable Text implementation

template<typename T>
BindableText<T>::BindableText() : text(Config::font) {}

template<typename T>
void BindableText<T>::operator=(const T x) {}

template<typename T>
void BindableText<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(text,state);
}

template<>
void BindableText<int>::operator=(const int x)
{
    text.setString(std::to_string(x));
}
template<>
void BindableText<std::string>::operator=(const std::string s)
{
    text.setString(s);
}

}