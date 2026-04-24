#include "../../include/UI/GUI-element.h"

#include "../../include/global/global.h"
#include <string>
#include <math.h>
#include <iostream>

namespace GUI
{

std::vector<DebugDot> debugDots;

//======================================================//

// Debug Dot implementation

DebugDot::DebugDot(sf::Vector2f pos, sf::Color color)
{
    dot.setOrigin(dot.getLocalBounds().position + dot.getLocalBounds().size * 0.5f);
    dot.setPosition(pos);
    dot.setRadius(2);
    dot.setFillColor(color);
}

void DebugDot::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(dot);
}

//======================================================//

// Theme rectangle implementation

ThemeRectangle::ThemeRectangle(
    sf::Vector2f size,
    sf::Vector2f pos
) : rect(size)
{
    rect.setPosition(pos);
}

void ThemeRectangle::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (rect.getFillColor() != Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]) {
        rect.setFillColor(Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]);
    }
    target.draw(rect,state);
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
    int highlightThickness,
    sf::Color bgColor,
    sf::Color textColor,
    sf::Color borderColor,
    sf::Color highlightColor,
    sf::Color specialColor
) : text(Global::numberFont)
{
    this->pos = pos;
    this->bgColor = bgColor;
    this->specialColor = specialColor;

    this->circle.setRadius(radius);
    this->circle.setOrigin(this->circle.getLocalBounds().position + this->circle.getLocalBounds().size * 0.5f);
    this->circle.setOutlineThickness(borderThickness);
    this->circle.setOutlineColor(borderColor);
    this->circle.setFillColor(bgColor);

    this->highlightOutline.setRadius(radius+2);
    this->highlightOutline.setOrigin(this->highlightOutline.getLocalBounds().position + this->highlightOutline.getLocalBounds().size * 0.5f);
    this->highlightOutline.setOutlineThickness(highlightThickness);
    this->highlightOutline.setOutlineColor(highlightColor);
    this->highlightOutline.setFillColor(sf::Color(0,0,0,0));

    this->text.setCharacterSize(characterSize);
    this->text.setString(text);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setFillColor(textColor);

    
    this->circle.setPosition(pos);
    this->highlightOutline.setPosition(pos);
    this->text.setPosition(pos);
}

void Node::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (this->circle.getOutlineColor() != Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]) {
        this->circle.setOutlineColor(Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]);
    }
    if (this->isSpecial && specialColor != Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::SPECIAL]) {
        specialColor = Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::SPECIAL];
        this->circle.setFillColor(specialColor);
    }
    target.draw(this->circle, state);
    target.draw(this->text, state);
    if (this->isHighlighted) target.draw(this->highlightOutline);
}

void Node::setPos(sf::Vector2f newPos)
{
    this->circle.setPosition(newPos);
    this->text.setPosition(newPos);
    this->highlightOutline.setPosition(newPos);
}

std::string Node::getValue()
{
    return this->text.getString();
}

void Node::setValue(std::string value)
{
    // std::cerr << "Set value called\n";
    this->text.setString(value);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition(this->circle.getPosition());
}

sf::Vector2f Node::getPos()
{
    return this->circle.getPosition();
}

void Node::setHighlighted(bool value)
{
    isHighlighted = value;
}

void Node::setSpecial(bool value)
{
    if (value) {
        this->circle.setFillColor(specialColor);
    }
    else {
        this->circle.setFillColor(bgColor);
    }
}

//======================================================//

// Line implementation

Line::Line(
    sf::Vector2f startPos,
    sf::Vector2f endPos,
    bool directed,
    std::string text,
    int lineThickness,
    int textSize,
    int textOutlineThickness,
    sf::Color lineColor,
    sf::Color highlightColor,
    sf::Color specialColor,
    sf::Color textColor,
    sf::Color textOutlineColor
) : startPos(startPos), endPos(endPos), lineThickness(lineThickness), text(Global::numberFont)
{
    this->line.setFillColor(lineColor);
    this->line.setOrigin({0,this->line.getLocalBounds().position.y + lineThickness * 0.5f});

    // if (text == "") this->text.setString("a");
    this->text.setString(text);
    this->text.setCharacterSize(textSize);
    this->text.setOutlineThickness(textOutlineThickness);
    this->text.setFillColor(textColor);
    this->text.setOutlineColor(textOutlineColor);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->textBg.setSize(this->text.getLocalBounds().size);
    this->textBg.setOrigin(this->textBg.getLocalBounds().position + this->textBg.getLocalBounds().size * 0.5f);
    this->textBg.setFillColor(sf::Color::White);
    this->textBg.setPosition(this->text.getPosition());
    // this->text.setOrigin(
    //     {0,this->text.getLocalBounds().position.x + this->text.getLocalBounds().size.x * 0.5f}
    // );
    // if (text == "") this->text.setString("");

    this->arrowHead.setRadius(arrowRad);
    this->arrowHead.setPointCount(3);
    this->arrowHead.setFillColor(lineColor);
    // this->arrowHead.setOrigin(this->arrowHead.getLocalBounds().position + this->arrowHead.getLocalBounds().size * 0.5f);
    this->arrowHead.setOrigin(
        {this->arrowHead.getLocalBounds().position.x + this->arrowHead.getLocalBounds().size.x * 0.5f, 0}
    );

    this->normalColor = lineColor;
    this->highlightColor = highlightColor;
    this->specialColor = specialColor;

    this->directed = directed;

    this->setupLine();
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (specialColor != Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]) {
        specialColor = Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN];
        if (isSpecial && !isHighlight) {
            if (arrowHead.getFillColor() != specialColor) {
                arrowHead.setFillColor(specialColor);
            }
            if (line.getFillColor() != specialColor) {
                line.setFillColor(specialColor);
            }
        }
    }
    if (text.getFillColor() != Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]) {
        text.setFillColor(Global::colorSet[Global::curColorSet][Global::COLOR_TYPE::MAIN]);
    }
    target.draw(this->line,state);
    target.draw(textBg);
    if (this->directed) target.draw(this->arrowHead,state);
    target.draw(this->text,state);
}

void Line::setupLine()
{
    double dx = this->endPos.x - this->startPos.x;
    double dy = this->endPos.y - this->startPos.y;
    float length = sqrt(dx * dx + dy * dy);

    this->line.setPosition(this->startPos);
    this->line.setSize({length,lineThickness});
    this->arrowHead.setPosition(this->endPos);

    sf::Angle angle(sf::radians(atan2(dy,dx)));

    this->line.setRotation(angle);
    this->arrowHead.setRotation(angle + sf::radians(asin(1)));
    
    // debugDots.emplace_back(this->line.getGlobalBounds().position);
    // debugDots.emplace_back(this->line.getGlobalBounds().position + this->line.getGlobalBounds().size);

    this->text.setPosition((this->startPos + this->endPos) * 0.5f);
    this->textBg.setPosition(this->text.getPosition());
}

void Line::setHighlight(bool val)
{
    isHighlight = val;
    if (val) {
        this->line.setFillColor(highlightColor);
        this->arrowHead.setFillColor(highlightColor);
    }
    else {
        setSpecial(isSpecial);
    }
}
void Line::setSpecial(bool val)
{
    isSpecial = val;
    if (val) {
        this->line.setFillColor(specialColor);
        this->arrowHead.setFillColor(specialColor);
    }
    else {
        this->line.setFillColor(normalColor);
        this->arrowHead.setFillColor(normalColor);
    }
}


void Line::setString(const std::string &s)
{
    this->text.setString(s);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);
    this->text.setPosition((this->startPos + this->endPos) * 0.5f);
    this->textBg.setSize(this->text.getLocalBounds().size);
    this->textBg.setOrigin(this->textBg.getLocalBounds().position + this->textBg.getLocalBounds().size * 0.5f);
    this->textBg.setFillColor(sf::Color::White);
    this->textBg.setPosition((this->startPos + this->endPos) * 0.5f);
}

void Line::setStartPos(sf::Vector2f newStartPos)
{
    this->startPos = newStartPos;
    this->setupLine();
}
void Line::setEndPos(sf::Vector2f newEndPos)
{
    this->endPos = newEndPos;
    this->setupLine();
}

sf::Vector2f Line::getStartPos()
{
    return this->startPos;
}
sf::Vector2f Line::getEndPos()
{
    return this->endPos;
}

}