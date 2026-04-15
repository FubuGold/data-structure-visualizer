#ifndef UI_GUI_ELEMENT_H
#define UI_GUI_ELEMENT_H

#include <SFML/Graphics.hpp>
#include <map>
#include "../global/global.h"

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
    DebugDot(sf::Vector2f pos, sf::Color color = sf::Color::Red);
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
    ~ValueText() = default;

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
    sf::CircleShape highlightOutline;
    sf::Text text;

    sf::Vector2f pos;
    sf::Color bgColor;
    sf::Color specialColor;

    bool isHighlighted;
    bool isSpecial;

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
        int highlightThickness = 0,
        sf::Color bgColor = sf::Color::Transparent,
        sf::Color textColor = Global::colorSet[0][Global::COLOR_TYPE::NETURAL],
        sf::Color borderColor = Global::colorSet[0][Global::COLOR_TYPE::MAIN],
        sf::Color highlightedColor = Global::colorSet[0][Global::COLOR_TYPE::HIGHLIGHT],
        sf::Color specialColor = Global::colorSet[0][Global::COLOR_TYPE::SPECIAL]
    );
    ~Node() = default;

    void setPos(sf::Vector2f newPos);
    std::string getValue();
    void setValue(std::string value);
    sf::Vector2f getPos();
    void setHighlighted(bool value);
    void setSpecial(bool value);
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

    static constexpr float arrowRad = 6;

    sf::CircleShape arrowHead;
    sf::Vector2f startPos,endPos;
    sf::RectangleShape line;
    int lineThickness;

    sf::Color normalColor, highlightColor;
    sf::Text text;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void setupLine();

    bool directed = true;

public:

    Line(
        sf::Vector2f startPos,
        sf::Vector2f endPos,
        std::string text = "",
        int lineThickness = 3,
        int textSize = 20,
        int textOutlineThickness = 2,
        sf::Color lineColor = sf::Color::Black,
        sf::Color highlightColor = Global::colorSet[0][Global::COLOR_TYPE::HIGHLIGHT],
        sf::Color textColor = Global::colorSet[0][Global::COLOR_TYPE::MAIN],
        sf::Color textOutlineColour = sf::Color::White,
        bool directed = true
    );
    ~Line() = default;

    void highlight();
    void unhighlight();

    void setString(const std::string &s);

    void setStartPos(sf::Vector2f newStartPos);
    void setEndPos(sf::Vector2f newEndPos);
    
    sf::Vector2f getStartPos();
    sf::Vector2f getEndPos();
};

//======================================================//

}

#include "GUI-element.ipp"

#endif // UI_GUI_ELEMENT_H