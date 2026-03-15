#ifndef UI_GUI_ELEMENT_H
#define UI_GUI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include <map>
#include "../global/config.h"

namespace GUI
{

//======================================================//

/**
 * @brief Debug point to render when testing
 * 
 */
class DebugDot : public sf::Drawable, public sf::Transformable
{
protected:
    sf::CircleShape dot;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    DebugDot(sf::Vector2f pos);
};

extern std::vector<DebugDot> debugDots;

//======================================================//

/**
 * @brief Text that can change the value
 * 
 * @tparam T Support only in and string (both std and sf)
 */
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

//======================================================//

/**
 * @brief Node with predetermined value
 * 
 */
class Node : public sf::Drawable, public sf::Transformable
{
protected:
    sf::CircleShape circle;
    sf::Text text;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:
    // Position is for the center
    Node(
        sf::Vector2f pos,
        std::string text = "",
        int radius = 15,
        int characterSize = 15,
        int textOutline = 0,
        int borderThickness = 0,
        sf::Color bgColor = sf::Color(0,200,0),
        sf::Color textColor = sf::Color::Black,
        sf::Color borderColor = sf::Color::Black
    );
    ~Node() = default;

    void setPos(sf::Vector2f newPos);
    std::string getValue();
    void setValue(std::string value);
    sf::Vector2f getPos();
};

//======================================================//

/**
 * @brief Line between two point
 * 
 * Mostly used for tree and graph rendering
 * 
 */
class Line : public sf::Drawable, public sf::Transformable
{
protected:
    sf::Vector2f startPos,endPos;
    sf::RectangleShape line;
    int lineThickness;

    sf::Text text;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void setupLine();

public:
    Line(
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
    ~Line() = default;

    void setStartPos(sf::Vector2f newStartPos);
    void setEndPos(sf::Vector2f newEndPos);
    
    sf::Vector2f getStartPos();
    sf::Vector2f getEndPos();
};

//======================================================//

}

#include "GUI-element.ipp"

#endif // UI_GUI_ELEMENT_H