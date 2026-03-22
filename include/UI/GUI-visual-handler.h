#ifndef GUI_VISUAL_HANDLER
#define GUI_VISUAL_HANDLER
#include "GUI-element.h"
#include "GUI-interactable-element.h"

namespace GUI
{

//======================================================//

class Animation
{
protected:
    sf::Vector2f vectorVel; // (pixel / s)
    const float defaultDuration = 0.5;

    float duration = 0;
    sf::Vector2f startPos, endPos;
    Node *object;

    sf::Angle angle;

public:

    bool isDone();
    void play(Node *object, sf::Vector2f startPos, sf::Vector2f endPos);
    void update();
    void end();

};


//======================================================//

/**
 * @brief Node and edge handler for tree
 * 
 * 
 */
class TreeVisualHandler : public sf::Drawable, public sf::Transformable
{
protected:
    static constexpr float nodeRadius = 20;
    static constexpr float padding = 10;
    static constexpr float offsetUp = 0.2;

    class TreeNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        TreeNode() : Node({0,0},"",nodeRadius) {};
        int paId = -1;
        int height = -1;
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

    sf::RectangleShape background;

    sf::Vector2f pos, size;

    float centerLine;
    float startY, stepY;

    int curTreeHeight = 0;

    int root = 0;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void recalHeightRecur(int id,int heigth = 0);
    void recalHeight();
    void recalPos(int id,int cnt = 1);

    std::vector<Line> lineList;
    std::vector<Animation*> animationList;
    void recalLine();

public:
    // The position is at the top left conner ({0,0})
    TreeVisualHandler(
        sf::Vector2f size,
        sf::Vector2f pos,
        int numHeight = 6
    );
    ~TreeVisualHandler() = default;

    void setTreeStructure(Global::TreeStructure &newStructure);

    void updateAnimation();

    void endAnimation();

    bool isAnimationEnd();

    void clear();

};

//======================================================//

}

#endif // GUI_VISUAL_HANDLER