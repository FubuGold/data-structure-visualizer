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
    static constexpr float nodeRadius = 14;
    static constexpr float padding = 7;

    class TreeNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        TreeNode() : Node({0,0},"",nodeRadius,14,0,3,2) {};
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

    TreeEdge calLinePosition(sf::Vector2f from, sf::Vector2f to);
    void recalLine();

public:
    /**
     * @brief Construct a new Tree Visual Handler object
     * 
     * @param size 
     * @param pos 
     * @param stepY 
     * @param startY is relative to the top left corner of the visual
     */
    TreeVisualHandler(sf::Vector2f size, sf::Vector2f pos, int stepY = 80, int startY = 51);
    ~TreeVisualHandler() = default;

    void setTreeStructure(Global::TreeStructure &newStructure);

    void updateAnimation();

    void endAnimation();

    bool isAnimationEnd();

    void clear();

};

//======================================================//

class CodeVisualHandler : public sf::Drawable, public sf::Transformable
{
protected:

    std::vector<std::vector<std::string>> codes;
    std::vector<std::string> funcNames;

    GUI::RectangleButton title;
    std::vector<sf::Text> codeTexts;
    sf::RectangleShape bg;
    std::vector<sf::RectangleShape> codeBg;

    int charSize = 14;
    int curFunc = -1;
    int curLine = -1;

    sf::Vector2f pos, lineSize;
    // int maxLine,numFunc;

    void loadCode(const std::string &filename);

    void generateVisual();
    
    void highlightCur();
    void unhighlightCur();

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:

    CodeVisualHandler(
        sf::Vector2f pos,
        sf::Vector2f lineSize,
        int charSize,
        std::vector<std::string> funcNames,
        std::vector<std::string> filenames
    );
    ~CodeVisualHandler() = default;

    void setFunc(int func);
    void setLine(int line);

};

}

#endif // GUI_VISUAL_HANDLER