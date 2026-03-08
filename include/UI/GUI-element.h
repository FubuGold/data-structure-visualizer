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

/**
 * @brief Node and edge handler for tree
 * 
 * This does not support trie
 * 
 */
class TreeHandler : public sf::Drawable, public sf::Transformable
{
protected:
    class TreeNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        TreeNode() : Node({0,0}) {};
        int paId = -1;
        int layer = -1;
        int leftCh = -1, rightCh = -1;
    };
    class TreeEdge : public Line
    {
    public:
        using Line::Line;
        TreeEdge() : Line({0,0},{0,0}) {};
        int fromId = -1, toId = -1;
    };

    std::map<int,TreeNode> nodeList;
    std::map<int,TreeEdge> edgeList;
    
    sf::RectangleShape background;

    sf::Vector2f pos, size;

    const int nodeRadius = 20;
    const float offsetUp = 0.2;
    float centerLine;
    float startY, stepY;

    int curLayer = 0;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void recalPos(int id,int cnt = 1);

public:
    // The position is at the top left conner ({0,0})
    TreeHandler(
        sf::Vector2f size,
        sf::Vector2f pos,
        int numLayer = 6
    );
    ~TreeHandler() = default;

    void insert(int id, int pa, bool left, std::string value);
    // This will ONLY remove the node and all associated edge.
    void remove(int id) {};
    void swap(int id1,int id2) {};
    void clear();

};

}

#include "GUI-element.ipp"

#endif // UI_GUI_ELEMENT_H