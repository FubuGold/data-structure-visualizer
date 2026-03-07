#include "../../include/UI/GUI-element.h"

#include "../../include/global/config.h"
#include <string>
#include <math.h>
#include <iostream>

namespace GUI
{

std::vector<DebugDot> debugDots;

//======================================================//

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

//======================================================//

// Value text specific implementation

template<>
void ValueText<int>::operator=(const int x)
{
    text.setString(std::to_string(x));
}
template<>
void ValueText<std::string>::operator=(const std::string s)
{
    text.setString(s);
}
template<>
void ValueText<sf::String>::operator=(const sf::String s)
{
    text.setString(s);
}

//======================================================//

// Node implementation

Node::Node(
    sf::Vector2f pos,
    std::string text,
    int radius,
    int characterSize,
    int textOutline,
    int borderThickness,
    sf::Color bgColor,
    sf::Color textColor,
    sf::Color borderColor
) : text(Config::font)
{
    this->circle.setRadius(radius);
    this->circle.setOrigin(this->circle.getLocalBounds().position + this->circle.getLocalBounds().size * 0.5f);
    this->circle.setOutlineThickness(borderThickness);
    this->circle.setOutlineColor(borderColor);
    this->circle.setFillColor(bgColor);
    
    this->text.setCharacterSize(characterSize);
    this->text.setString(text);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setFillColor(textColor);

    this->circle.setPosition(pos);
    this->text.setPosition(pos);
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->circle, state);
    target.draw(this->text, state);
}

void Node::setPos(sf::Vector2f newPos)
{
    this->circle.setPosition(newPos);
    this->text.setPosition(newPos);
}

//======================================================//

// Edge implementation

Edge::Edge(
    sf::Vector2f startPos,
    sf::Vector2f endPos,
    std::string text,
    int lineThickness,
    int textSize,
    int textOutlineThickness,
    sf::Color lineColor,
    sf::Color textColor,
    sf::Color textOutlineColor
) : startPos(startPos), endPos(endPos), lineThickness(lineThickness), text(Config::font)
{
    this->line.setFillColor(lineColor);
    this->line.setOrigin({this->line.getLocalBounds().position.x + lineThickness * 0.5f, 0});

    this->text.setString(text);
    this->text.setCharacterSize(textSize);
    this->text.setOutlineThickness(textOutlineThickness);
    this->text.setFillColor(textColor);
    this->text.setOutlineColor(textOutlineColor);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);

    this->setupLine();
}

void Edge::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->line,state);
    target.draw(this->text,state);
}

void Edge::setupLine()
{
    double dx = this->endPos.x - this->startPos.x;
    double dy = this->endPos.y - this->startPos.y;
    float length = sqrt(dx * dx + dy * dy);
    this->line.setPosition(this->startPos);
    this->line.setSize({length,lineThickness});
    this->line.setRotation(sf::radians(atan2(dy,dx)));

    this->text.setPosition(this->line.getGlobalBounds().position + this->line.getGlobalBounds().size * 0.5f);
    
    // sf::Vector2f tmp = this->line.getGlobalBounds().position + this->line.getGlobalBounds().size * 0.5f;
    // debugDots.push_back(DebugDot(this->line.getGlobalBounds().position));
    // debugDots.push_back(DebugDot(this->line.getGlobalBounds().position + this->line.getGlobalBounds().size));
    // debugDots.push_back(DebugDot(tmp));
}

void Edge::setStartPos(sf::Vector2f newStartPos)
{
    this->startPos = newStartPos;
    this->setupLine();
}

void Edge::setEndPos(sf::Vector2f newEndPos)
{
    this->endPos = newEndPos;
    this->setupLine();
}

}