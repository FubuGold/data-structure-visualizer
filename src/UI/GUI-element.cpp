#include "../../include/UI/GUI-element.h"

#include "../../include/config/config.h"
#include <string>

namespace GUI
{

// Bindable Text implementation

template<typename T>
BindableText<T>::BindableText() : text(Config::font) {}

template<typename T>
void BindableText<T>::operator=(const T x) {}

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

template<typename T>
void BindableText<T>::draw(sf::RenderTarget& target, sf::RenderStates state)
{
    target.draw(text,state);
}

}