#include "../../include/UI/GUI-visual-handler.h"
#include <math.h>

namespace GUI
{

//======================================================//

// Animation implementation

bool Animation::isDone()
{
    return duration == 0;
}

void Animation::play(Node *object, sf::Vector2f startPos, sf::Vector2f endPos)
{
    this->object = object;
    this->startPos = startPos;
    this->endPos = endPos;
    this->duration = this->defaultDuration;

    this->angle = sf::Angle(sf::radians( atan2((endPos-startPos).y, (endPos-startPos).x) ));
    vectorVel = sf::Vector2f((startPos - endPos).length() / this->duration,angle);
}

void Animation::update()
{
    float cappedDeltaTime;
    // std::cerr << Global::deltaTime << ' ' << this->duration << '\n';
    if (this->duration <= 0) return;
    if (this->duration > Global::deltaTime) {
        cappedDeltaTime = Global::deltaTime;
        this->duration -= Global::deltaTime;
    }
    else {
        cappedDeltaTime = this->duration;
        this->duration = 0;
    }
    sf::Vector2f newPos = object->getPos() + sf::Vector2f(vectorVel * cappedDeltaTime);
    object->setPos(newPos);
}

void Animation::end()
{
    this->object->setPos(this->endPos);
    duration = 0;
}

//======================================================//

// Tree handler implementation

std::pair<int,int> getId(int x,int y) {return {std::min(x,y),std::max(x,y)}; }

TreeVisualHandler::TreeVisualHandler(sf::Vector2f size, sf::Vector2f pos, int numHeight)
{
    this->pos = pos;
    this->size = size;
    this->background.setSize(size);
    this->background.setPosition(pos);
    this->background.setFillColor(sf::Color::White);

    centerLine = size.x / 2;
    stepY = size.y / (numHeight + 1);
    startY = stepY * (1 - offsetUp);

    debugDots.emplace_back(pos);
    debugDots.emplace_back(pos + size);
    debugDots.emplace_back(pos + sf::Vector2f(centerLine, startY));
}

void TreeVisualHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->background);
    for (int i=0;i<lineList.size();i++) {
        target.draw(lineList[i], state);
    }

    for (const std::pair<int,TreeNode>& node : this->nodeList)
    {
        target.draw(node.second, state);
    }
}

void TreeVisualHandler::recalHeightRecur(int id,int height) 
{
    if (id == -1) return;
    TreeNode &tmp = nodeList[id];
    tmp.height = height;
    this->curTreeHeight = std::max(height,this->curTreeHeight);
    if (tmp.leftCh != -1) recalHeightRecur(tmp.leftCh,height + 1); 
    if (tmp.rightCh != -1) recalHeightRecur(tmp.rightCh,height + 1); 
}

void TreeVisualHandler::recalHeight()
{
    this->curTreeHeight = 0;
    recalHeightRecur(this->root);
}

void TreeVisualHandler::recalPos(int id,int cnt)
{
    if (id == -1) return;
    std::cerr << "Recalculate position of: " << id << " of count: " << cnt << '\n';
    assert(nodeList.find(id) != nodeList.end());
    TreeNode &cur = nodeList[id];

    sf::Vector2f prevPos = cur.getPos(), newPos;
    
    // std::cerr << "Node accquired";

    if (cnt == 1) {
        newPos = this->pos + sf::Vector2f(centerLine, startY);
    }
    else {
        int numNode = 1 << curTreeHeight;
        float startX = centerLine - (nodeRadius * numNode) - (padding * (numNode - 1) / 2);
        float endX = startX + (nodeRadius * 2 * numNode) + padding * (numNode - 1);
        float stepX = (endX - startX - padding * ((1 << cur.height) - 1)) / (1 << cur.height);

        int heightNodeIdx = cnt - (1 << cur.height) + 1;

        std::cerr << "Height node debug: " << cnt << ' ' << cur.height << ' ' << '\n';

        std::cerr << "Debug number: " << numNode << ' ' << startX << ' ' << endX << ' ' << stepX << ' ' << heightNodeIdx << '\n';

        startX = startX + stepX * (heightNodeIdx - 1) + padding * (heightNodeIdx - 1);
        newPos = this->pos + sf::Vector2f(startX + stepX / 2, startY + cur.height * stepY);
    }

    // std::cerr << "Position calculated " << id << '\n';
    // std::cerr << newPos.x << ' ' << newPos.y << '\n';
    // debugDots.emplace_back(newPos,sf::Color::Magenta);

    Animation *tmp = new Animation();
    tmp->play(&cur,prevPos,newPos);
    animationList.push_back(tmp);
    

    if (cur.leftCh != -1) {
        recalPos(cur.leftCh, cnt*2);
    }
    if (cur.rightCh != -1) {
        recalPos(cur.rightCh, cnt*2+1);
    }
}

void TreeVisualHandler::recalLine()
{
    lineList.clear();
    for (std::pair<const int,TreeNode> &node : nodeList) {
        if (node.second.leftCh != -1)
            lineList.push_back(TreeEdge(node.second.getPos(),
                            nodeList[node.second.leftCh].getPos()));
        
        if (node.second.rightCh != -1)
            lineList.push_back(TreeEdge(node.second.getPos(),
                            nodeList[node.second.rightCh].getPos()));
    }
}

void TreeVisualHandler::setTreeStructure(Global::TreeStructure &newStructure)
{
    std::cerr << newStructure << '\n';
    std::cerr << "Tree structure reset\n";
    this->root = newStructure.rootId;

    // Insert and update
    for (std::pair<const int,std::tuple<int,int,bool>> &newNode : newStructure.structureMap) {
        TreeNode &curNode = this->nodeList[newNode.first];
        curNode.leftCh = std::get<0>(newNode.second);
        curNode.rightCh = std::get<1>(newNode.second);
        curNode.setHighlighted(std::get<2>(newNode.second));
        if (curNode.leftCh != -1) this->nodeList[curNode.leftCh].paId = newNode.first;
        if (curNode.rightCh != -1) this->nodeList[curNode.rightCh].paId = newNode.first;
        
        curNode.setValue(newStructure.valueMap[newNode.first]);
    }

    // Clean up
    for (auto it = nodeList.begin(); it != nodeList.end();) {
        if (newStructure.structureMap.find(it->first) == newStructure.structureMap.end()) {
            it = nodeList.erase(it);
        }
        else it++;
    }

    std::cerr << "Tree structure reset complete\n";
    recalHeight();
    std::cerr << "Tree visual height: " << this->curTreeHeight << '\n';
    recalPos(this->root);
    std::cerr << "Recal pos completed\n";
    std::cerr << "======================================================\n";
}

void TreeVisualHandler::updateAnimation()
{
    for (Animation *&ani : animationList) {
        if (ani->isDone()) {
            delete ani;
            ani = nullptr;
        }
        else {
            ani->update();
        }
    }
    for (int i=0;i<animationList.size();i++) {
        if (animationList[i] == nullptr) {
            i = animationList.erase(animationList.begin() + i) - animationList.begin() - 1;
        }
    }
    recalLine();
}

void TreeVisualHandler::endAnimation()
{
    for (Animation *&ani : animationList) {
        ani->end();
    }
}

bool TreeVisualHandler::isAnimationEnd()
{
    return animationList.empty();
}

void TreeVisualHandler::clear()
{
    lineList.clear();
    nodeList.clear();
    curTreeHeight = 0;
}

}