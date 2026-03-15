#include "GUI-element.h"
#include "GUI-interactable-element.h"

namespace GUI
{

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
    std::map<int,int> layerNodeCnt;
    
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
    // This will ONLY remove the node and all associated edge. Only work for leaves
    void remove(int id);
    void swapValue(int id1,int id2);
    // Only recal if this is the last update in the sequence
    void updateChild(int id,int leftId,int rightId, bool recal = true);
    void clear();

};

enum class TreeCmdType
{
    Insert,
    Remove,
    SwapValue,
    UpdateChild,
    Clear
};

struct TreeCmd
{
    TreeCmdType type;

    int id;
    int pa;
    bool left;

    // For swap value
    int id2;

    int leftId;
    int rightId;

    std::string value;
};

//======================================================//

}