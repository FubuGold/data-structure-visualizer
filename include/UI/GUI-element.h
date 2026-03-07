#ifndef UI_GUI_ELEMENT_H
#define UI_GUI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include "../global/config.h"

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

extern std::vector<DebugDot> debugDots;

template<typename T>
class ValueText : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Text text;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    ValueText(
        sf::Vector2f pos,
        int characterSize = 15,
        int textOutline = 0,
        sf::Color textColor = sf::Color::Black
    );

    void operator=(const T x);

};

class Node : public sf::Drawable, public sf::Transformable
{
protected:
    sf::CircleShape circle;
    sf::Text text;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    Node(
        sf::Vector2f pos,
        std::string text = "",
        int radius = 15,
        int characterSize = 10,
        int textOutline = 0,
        int borderThickness = 0,
        sf::Color bgColor = sf::Color(0,128,0),
        sf::Color textColor = sf::Color::Black,
        sf::Color borderColor = sf::Color::Black
    );
    ~Node() = default;

    void setPos(sf::Vector2f newPos);
};

class Edge : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector2f startPos,endPos;
    sf::RectangleShape line;
    int lineThickness;

    sf::Text text;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void setupLine();

public:
    Edge(
        sf::Vector2f startPos,
        sf::Vector2f endPos,
        std::string text = "",
        int lineThickness = 5,
        int textSize = 20,
        int textOutlineThickness = 2,
        sf::Color lineColor = sf::Color::Black,
        sf::Color textColor = sf::Color::Black,
        sf::Color textOutlineColour = sf::Color::White
    );
    ~Edge() = default;

    void setStartPos(sf::Vector2f newStartPos);
    void setEndPos(sf::Vector2f newEndPos);

};

}

#include "GUI-element.ipp"

#endif // UI_GUI_ELEMENT_H