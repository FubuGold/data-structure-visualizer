// Bindable Text implementation

#include <SFML/Graphics.hpp>
#include "../global/config.h"

namespace GUI
{

//======================================================//

// Value text template implementation

template<typename T>
ValueText<T>::ValueText(
    sf::Vector2f pos,
    int characterSize,
    int textOutline,
    sf::Color textColor
) : text(Config::font)
{
    text.setPosition(pos);
    text.setCharacterSize(characterSize);
    text.setOutlineThickness(textOutline);
    text.setFillColor(textColor);
}

template<typename T>
void ValueText<T>::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(text,state);
}

}