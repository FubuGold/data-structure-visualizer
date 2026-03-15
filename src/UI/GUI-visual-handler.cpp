#include "../../include/UI/GUI-visual-handler.h"

namespace GUI
{

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
    layerNodeCnt[curNode.layer]++;
    curLayer = layerNodeCnt.rbegin()->first;
    if (pa >= 0) {
        if (left) nodeList[pa].leftCh = id;
        else nodeList[pa].rightCh = id;
    }
    nodeList[id] = curNode;

    edgeList.clear();
    recalPos(0);
    std::cerr << std::endl;
}

void TreeHandler::remove(int id)
{
    int nodeLayer = nodeList[id].layer;
    int &tmp = layerNodeCnt[nodeLayer];
    tmp--;
    if (tmp == 0) {
        layerNodeCnt.erase(nodeLayer);
        curLayer = layerNodeCnt.rbegin()->first;
    }
    nodeList.erase(id);
    for (auto &p : nodeList) {
        TreeNode &tmp = p.second;
        if (tmp.leftCh == id) tmp.leftCh = -1;
        else if (tmp.rightCh == id) tmp.rightCh = -1;
    }
    for (auto it = edgeList.begin(); it != edgeList.end();) {
        if (it->second.fromId == id || it->second.toId == id) {
            it = edgeList.erase(it);
        }
        else it++;
    }
}

void TreeHandler::swapValue(int id1,int id2)
{
    TreeNode &node1 = nodeList[id1], &node2 = nodeList[id2];
    std::string tmp = node1.getValue();
    node1.setValue(node2.getValue());
    node2.setValue(tmp);
}

void TreeHandler::updateChild(int id,int leftId,int rightId, bool recal)
{
    assert(nodeList.size());
    TreeNode &tmp = nodeList[id];
    tmp.leftCh = leftId;
    tmp.rightCh = rightId;

    if (recal) recalPos(nodeList.begin()->first);
}

void TreeHandler::clear()
{
    nodeList.clear();
    edgeList.clear();
    curLayer = 0;
}

}