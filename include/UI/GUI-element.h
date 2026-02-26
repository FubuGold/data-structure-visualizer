#ifndef UI_GUI_ELEMENT_H
#define UI_GUI_ELEMENT_H

#include <SFML/Graphics.hpp>

namespace GUI
{

class DebugDot : public sf::Drawable, public sf::Transformable
{
protected:
    sf::CircleShape dot;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    DebugDot(sf::Vector2f pos);


};

template<typename T>
class BindableText : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Text text;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    BindableText();

    void operator=(const T x);

};

class Node : public sf::Drawable, public sf::Transformable
{

};

class Edge : public sf::Drawable, public sf::Transformable
{

};

};

#endif // UI_GUI_ELEMENT_H