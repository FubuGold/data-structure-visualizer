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

sf::Vector2f Node::getPos()
{
    return this->circle.getPosition();
}

//======================================================//

// Line implementation

Line::Line(
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
    this->line.setOrigin({0,this->line.getLocalBounds().position.y + lineThickness * 0.5f});

    this->text.setString(text);
    this->text.setCharacterSize(textSize);
    this->text.setOutlineThickness(textOutlineThickness);
    this->text.setFillColor(textColor);
    this->text.setOutlineColor(textOutlineColor);
    this->text.setOrigin(this->text.getLocalBounds().position + this->text.getLocalBounds().size * 0.5f);

    this->setupLine();
}

void Line::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->line,state);
    target.draw(this->text,state);
}

void Line::setupLine()
{
    double dx = this->endPos.x - this->startPos.x;
    double dy = this->endPos.y - this->startPos.y;
    float length = sqrt(dx * dx + dy * dy);
    this->line.setPosition(this->startPos);
    this->line.setSize({length,lineThickness});
    this->line.setRotation(sf::radians(atan2(dy,dx)));

    this->text.setPosition(this->line.getGlobalBounds().position + this->line.getGlobalBounds().size * 0.5f);

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

//======================================================//

// Tree handler implementation

TreeHandler::TreeHandler(sf::Vector2f size, sf::Vector2f pos, int numLayer)
{
    this->pos = pos;
    this->size = size;
    this->background.setSize(size);
    this->background.setPosition(pos);
    this->background.setFillColor(sf::Color::White);

    centerLine = size.x / 2;
    stepY = size.y / (numLayer + 1);
    startY = stepY * (1 - offsetUp);
    

    debugDots.emplace_back(pos);
    debugDots.emplace_back(pos + size);
    debugDots.emplace_back(pos + sf::Vector2f(centerLine, startY));
}

void TreeHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->background);
    for (const std::pair<int,TreeEdge>& edge : this->edgeList)
    {
        target.draw(edge.second);
    }
    for (const std::pair<int,TreeNode>& node : this->nodeList)
    {
        target.draw(node.second);
    }
}

void TreeHandler::recalPos(int id,int cnt)
{
    TreeNode &cur = nodeList[id];

    if (cnt == 1) {
        cur.setPos(this->pos + sf::Vector2f(centerLine, startY));
    }
    else {
        float startX = centerLine - (nodeRadius << curLayer);
        float endX = startX + (nodeRadius << (curLayer + 1));
        float stepX = (endX - startX) / (1 << cur.layer);

        int layerNodeIdx = cnt - (1 << cur.layer) + 1;
        startX = startX + stepX * (layerNodeIdx - 1);
        cur.setPos(pos + sf::Vector2f(startX + stepX / 2, startY + cur.layer * stepY));
        sf::Vector2f tmp = pos + sf::Vector2f(startX + stepX / 2, startY + cur.layer * stepY);

    }

    if (cur.leftCh != -1) {
        recalPos(cur.leftCh, cnt*2);

        TreeEdge edge(cur.getPos(), nodeList[cur.leftCh].getPos());
        edge.fromId = id;
        edge.toId = cur.leftCh;
        edgeList[cnt*2] = edge;
    }
    if (cur.rightCh != -1) {
        recalPos(cur.rightCh, cnt*2+1);

        TreeEdge edge(cur.getPos(), nodeList[cur.rightCh].getPos());
        edge.fromId = id;
        edge.toId = cur.rightCh;
        edgeList[cnt*2+1] = edge;
    }

}

void TreeHandler::insert(int id, int pa, bool left, std::string value)
{
    TreeNode curNode({0,0}, value, nodeRadius, nodeRadius);
    curNode.layer = pa < 0 ? 0 : nodeList[pa].layer + 1;
    curNode.paId = pa;
    curLayer = std::max(curNode.layer,curLayer);
    if (pa >= 0) {
        if (left) nodeList[pa].leftCh = id;
        else nodeList[pa].rightCh = id;
    }
    nodeList[id] = curNode;

    edgeList.clear();
    recalPos(0);
    std::cerr << std::endl;
}

void TreeHandler::clear()
{
    nodeList.clear();
    edgeList.clear();
    curLayer = 0;
}

}