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
    static constexpr float nodeRadius = 15;
    static constexpr float padding = 8;

    class TreeNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        TreeNode() : Node({0,0},"",nodeRadius,14,0,3,2) {};
        int paId = -1;
        int height = -1;
        int leftCh = -1, rightCh = -1;
        bool isHighlighted = 0;
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

    std::vector<TreeEdge> lineList;
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
    ~TreeVisualHandler();

    void setTreeStructure(const Global::TreeStructure &newStructure);

    void updateAnimation();

    void endAnimation();

    bool isAnimationEnd();

    void clear();

};

//======================================================//

class SLLVisualHandler : public sf::Drawable, public sf::Transformable
{
protected:
    static constexpr float nodeRadius = 15;
    // static constexpr float padding = 7;

    class StructNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        StructNode() : Node({0,0},"",nodeRadius,15,0,3,2) {};
        int prevId = -1;
        int nxt = -1;
        int isHighlighted = 0;
    };
    class StructLine : public Line
    {
    public:
        using Line::Line;
        StructLine() : Line({0,0},{0,0}) {};
        int fromId = -1, toId = -1;
    };

    std::map<int,StructNode> nodeList;

    sf::RectangleShape background;

    sf::Vector2f pos, size;

    float yLine, startX, stepX;

    int curTreeHeight = 0;

    int root = 0;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    void recalPos();

    std::vector<StructLine> lineList;
    std::vector<Animation*> animationList;

    StructLine calLinePosition(sf::Vector2f from, sf::Vector2f to);
    void recalLine();

public:
    /**
     * @brief Construct a new Tree Visual Handler object
     * 
     * @param size 
     * @param pos 
     * @param yLine is relative to the top left corner of the visual
     * @param startX is relative to the top left corner of the visual
     * @param stepX 
     */
    SLLVisualHandler(sf::Vector2f size, sf::Vector2f pos, int yLine = 80, int startX = 51,int stepX = 51);
    ~SLLVisualHandler();

    void setTreeStructure(const Global::TreeStructure &newStructure);

    void updateAnimation();

    void endAnimation();

    bool isAnimationEnd();

    void clear();

};

//======================================================//

class TrieVisualHandler : public sf::Drawable, public sf::Transformable
{
protected:
    static constexpr float nodeRadius = 15;
    static constexpr float padding = 30;

    class TrieNode : public Node
    {
    public:
        using Node::Node;
        using Node::getPos;
        TrieNode();
        int paId = -1;
        int ch[26];
        bool isHighlighted = 0;
    };
    class TrieEdge : public Line
    {
    public:
        using Line::Line;
        TrieEdge() : Line({0,0},{0,0},true,"",2,15) {};
        int fromId = -1, toId = -1;
    };

    std::map<int,TrieNode> nodeList;

    sf::RectangleShape background;

    sf::Vector2f pos, size;

    float centerLine, stepY, startY;

    int curTreeHeight = 0;

    int root = 0;
    
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    int numLeaves;
    int startX;
    void findStartX();
    int leafCnt;
    float recalPos(int id,int height);

    std::vector<TrieEdge> lineList;
    std::vector<Animation*> animationList;

    TrieEdge calLinePosition(sf::Vector2f from, sf::Vector2f to);
    void recalLine();

public:
    /**
     * @brief Construct a new Tree Visual Handler object
     * 
     * @param size 
     * @param pos 
     * @param yLine is relative to the top left corner of the visual
     * @param startX is relative to the top left corner of the visual
     * @param stepX 
     */
    TrieVisualHandler(sf::Vector2f size, sf::Vector2f pos, int stepY = 80, int startY = 51);
    ~TrieVisualHandler();

    void setTreeStructure(const Global::TreeStructure &newStructure);

    void updateAnimation();

    void endAnimation();

    bool isAnimationEnd();

    void clear();

};

//======================================================//

class GraphVisualHandler : public sf::Drawable, public sf::Transformable
{
protected:
    static constexpr float nodeRadius = 15;
    static constexpr float charSize = 15;

    sf::Vector2f pos, size;
    sf::Vector2f center;
    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

    int numNode;
    std::vector<Node> nodes;
    std::vector<sf::Vector2f> disp;
    std::vector<Line> lines;

    Global::GraphStructure baseStructure;

    // const float physDuration = 20; // seconds
    float temperature = 100;

    // The position is relative to the handler.
    // The actual position should be recalculated by other function.
    void simulation(float forceConst);
    void calPos();

public:

    GraphVisualHandler(sf::Vector2f size, sf::Vector2f pos);

    void buildGraph(const Global::GraphStructure &structure);

    void setCurrentState(const Global::GraphStructure &newStructure);

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

//======================================================//

/**
 * @brief This is a container that help handle a set of elements. This do not have a bounding box
 * 
 */
class PopupGroup : public IInteractableElement
{
protected:

    std::vector<std::shared_ptr<sf::Drawable>> elements;
    std::vector<std::shared_ptr<IInteractableElement>> iElements;

    bool isEnable = false;

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override;

public:

    void handleEvent(const std::optional<sf::Event> &e) override;
    bool containPos(sf::Vector2f pos) override;

    template<typename T>
    requires std::derived_from<T,sf::Drawable>
    void addElement(std::shared_ptr<T> element_ptr);
    void setWindow(sf::RenderTarget *target_ptr) override;

    void open();
    void close();

};

//======================================================//

/**
 * @brief Zoom view wrapper for elements. This will not store the element but will wrap the draw function
 * 
 */
class ZoomView : public IInteractableElement
{
private:
    sf::View view;
    sf::RenderWindow *window;

    float zoomFactor = 1.1f;
    bool dragging = false;
    sf::Vector2i lastMousePos;

public:
    ZoomView();

    void setCenter(const sf::Vector2f& center);
    void setSize(const sf::Vector2f& size);
    void setWindow(sf::RenderWindow *window);

    void handleEvent(const std::optional<sf::Event>& event);
    bool containPos(sf::Vector2f pos) override;

    template <typename T = sf::Drawable>
    requires std::derived_from<T,sf::Drawable>
    void draw(const T& element);

    void draw(sf::RenderTarget& target, sf::RenderStates state = sf::RenderStates::Default) const override {};
};

}

#include "GUI-visual-handler.ipp"

#endif // GUI_VISUAL_HANDLER