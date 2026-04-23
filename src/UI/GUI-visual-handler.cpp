#include "../../include/UI/GUI-visual-handler.h"

#include <math.h>
#include <fstream>
#include <random>
#include <chrono>

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
    // std::cerr << Global::deltaTime << ' ' << Global::animationSpeed << '\n';
    if (this->duration <= 0) return;
    if (this->duration > Global::deltaTime * Global::animationSpeed) {
        cappedDeltaTime = Global::deltaTime * Global::animationSpeed;
        this->duration -= Global::deltaTime * Global::animationSpeed;
    }
    else {
        cappedDeltaTime = this->duration;
        this->duration = 0;
    }
    sf::Vector2f newPos = object->getPos() + vectorVel * cappedDeltaTime;
    object->setPos(newPos);
}

void Animation::end()
{
    this->object->setPos(this->endPos);
    duration = 0;
}

//======================================================//

// Tree handler implementation

// std::pair<int,int> getId(int x,int y) {return {std::min(x,y),std::max(x,y)}; }

TreeVisualHandler::TreeVisualHandler(sf::Vector2f size, sf::Vector2f pos, int stepY, int startY)
{
    this->pos = pos;
    this->size = size;
    this->background.setSize(size);
    this->background.setPosition(pos);
    this->background.setFillColor(sf::Color::White);

    this->centerLine = size.x / 2;
    this->stepY = stepY;
    this->startY = startY;

    // debugDots.emplace_back(pos);
    // debugDots.emplace_back(pos + size);
    // debugDots.emplace_back(pos + sf::Vector2f(centerLine, startY));
}

TreeVisualHandler::~TreeVisualHandler()
{
    for (Animation *p : animationList) delete p;
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
    // std::cerr << "Recalculate position of: " << id << " of count: " << cnt << '\n';
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

        // std::cerr << "Height node debug: " << cnt << ' ' << cur.height << ' ' << '\n';

        // std::cerr << "Debug number: " << numNode << ' ' << startX << ' ' << endX << ' ' << stepX << ' ' << heightNodeIdx << '\n';

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

TreeVisualHandler::TreeEdge TreeVisualHandler::calLinePosition(sf::Vector2f from, sf::Vector2f to)
{
    // Calculate the vector from the center to the edge in the direction pointing to each other
    sf::Vector2f v1 = this->nodeRadius * (to - from).normalized();
    sf::Vector2f v2 = this->nodeRadius * (from - to).normalized();

    return TreeEdge(from + v1, to + v2);
}

void TreeVisualHandler::recalLine()
{
    lineList.clear();
    for (std::pair<const int,TreeNode> &node : nodeList) {
        if (node.second.leftCh != -1) {
            lineList.push_back(calLinePosition(node.second.getPos(),
                nodeList[node.second.leftCh].getPos()));

            lineList.back().setHighlight(
                node.second.isHighlighted && nodeList[node.second.leftCh].isHighlighted
            );
        }
        
        if (node.second.rightCh != -1) {
            lineList.push_back(calLinePosition(node.second.getPos(),
                nodeList[node.second.rightCh].getPos()));
            
            lineList.back().setHighlight(
                node.second.isHighlighted && nodeList[node.second.rightCh].isHighlighted
            );
        }
    }
}

void TreeVisualHandler::setTreeStructure(const Global::TreeStructure &newStructure)
{
    std::cerr << "Tree structure reset\n";
    // std::cerr << newStructure << '\n';
    this->root = newStructure.rootId;

    // Insert and update
    for (const std::pair<const int,std::tuple<int,int,bool>> &newNode : newStructure.structureMap) {
        TreeNode &curNode = this->nodeList[newNode.first];
        curNode.leftCh = std::get<0>(newNode.second);
        curNode.rightCh = std::get<1>(newNode.second);
        curNode.setHighlighted(std::get<2>(newNode.second));
        curNode.isHighlighted = std::get<2>(newNode.second);
        if (curNode.leftCh != -1) this->nodeList[curNode.leftCh].paId = newNode.first;
        if (curNode.rightCh != -1) this->nodeList[curNode.rightCh].paId = newNode.first;
        
        // std::cerr << newNode.first << ' ' << newStructure.valueMap[newNode.first] << '\n';

        curNode.setValue(newStructure.valueMap.at(newNode.first));
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
    // std::cerr << "Tree visual height: " << this->curTreeHeight << '\n';
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

//======================================================//

SLLVisualHandler::SLLVisualHandler(sf::Vector2f size, sf::Vector2f pos, int yLine, int startX, int stepX)
{
    this->pos = pos;
    this->size = size;
    this->background.setSize(size);
    this->background.setPosition(pos);
    this->background.setFillColor(sf::Color::White);

    this->yLine = yLine;
    this->startX = startX;
    this->stepX = stepX;
}

SLLVisualHandler::~SLLVisualHandler()
{
    for (Animation *p : animationList) delete p;
}

void SLLVisualHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->background);
    for (int i=0;i<lineList.size();i++) {
        target.draw(lineList[i], state);
    }

    for (const std::pair<int,StructNode>& node : this->nodeList)
    {
        target.draw(node.second, state);
    }
}

SLLVisualHandler::StructLine SLLVisualHandler::calLinePosition(sf::Vector2f from, sf::Vector2f to)
{
    // Calculate the vector from the center to the edge in the direction pointing to each other
    sf::Vector2f v1 = this->nodeRadius * (to - from).normalized();
    sf::Vector2f v2 = this->nodeRadius * (from - to).normalized();

    return StructLine(from + v1, to + v2);
}

void SLLVisualHandler::recalLine()
{
    lineList.clear();
    for (std::pair<const int,StructNode> &node : nodeList) {
        if (node.second.nxt != -1) {
            lineList.push_back(calLinePosition(node.second.getPos(),
                nodeList[node.second.nxt].getPos()));

            lineList.back().setHighlight(
                node.second.isHighlighted && nodeList[node.second.nxt].isHighlighted
            );
            
        }
    }
}

void SLLVisualHandler::recalPos()
{
    if (this->root == -1) return;

    sf::Vector2f lastPos(-1,-1);

    for (int curId = this->root; curId != -1; curId = nodeList[curId].nxt) {
        assert(nodeList.find(curId) != nodeList.end());
        StructNode &cur = nodeList[curId];
        sf::Vector2f prevPos = cur.getPos(), newPos;
        if (curId == this->root) {
            newPos = sf::Vector2f(startX, yLine) + pos;
        }
        else {
            newPos = lastPos + sf::Vector2f(stepX, 0);
        }

        Animation *tmp = new Animation();
        tmp->play(&cur,prevPos,newPos);
        animationList.push_back(tmp);

        lastPos = newPos;
    }
}

void SLLVisualHandler::setTreeStructure(const Global::TreeStructure &newStructure)
{
    std::cerr << "Tree structure reset\n";
    this->root = newStructure.rootId;

    for (const std::pair<const int,std::tuple<int,int,bool>> &newNode : newStructure.structureMap) {
        StructNode &curNode = this->nodeList[newNode.first];
        curNode.nxt = std::get<0>(newNode.second);
        curNode.setHighlighted(std::get<2>(newNode.second));
        curNode.isHighlighted = std::get<2>(newNode.second);
        if (curNode.nxt != -1) this->nodeList[curNode.nxt].prevId = newNode.first;

        curNode.setValue(newStructure.valueMap.at(newNode.first));
    }

    for (auto it = nodeList.begin(); it != nodeList.end();) {
        if (newStructure.structureMap.find(it->first) == newStructure.structureMap.end()) {
            it = nodeList.erase(it);
        }
        else it++;
    }

    std::cerr << "Tree structure reset complete\n";
    recalPos();
    std::cerr << "Recal pos completed\n";
    std::cerr << "===========================\n";
}

void SLLVisualHandler::updateAnimation()
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

void SLLVisualHandler::endAnimation()
{
    for (Animation *&ani : animationList) {
        ani->end();
    }
}

bool SLLVisualHandler::isAnimationEnd()
{
    return animationList.empty();
}

void SLLVisualHandler::clear()
{
    lineList.clear();
    nodeList.clear();
    curTreeHeight = 0;
}

//======================================================//

// Trie visual handler implementation

TrieVisualHandler::TrieVisualHandler(
    sf::Vector2f size,
    sf::Vector2f pos,
    int stepY,
    int startY
)
{
    this->pos = pos;
    this->size = size;
    this->background.setSize(size);
    this->background.setPosition(pos);
    this->background.setFillColor(sf::Color::White);

    this->centerLine = size.x / 2;
    this->stepY = stepY;
    this->startY = startY;
}

TrieVisualHandler::TrieNode::TrieNode() : Node({0,0},"",nodeRadius,15,0,3,2)
{
    for (int i = 0; i < 26; i++) ch[i] = -1;
}

TrieVisualHandler::~TrieVisualHandler()
{
    for (Animation *p : animationList) delete p;
}

void TrieVisualHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    target.draw(this->background);
    for (int i=0;i<lineList.size();i++) {
        target.draw(lineList[i], state);
    }

    for (const std::pair<int,TrieNode>& node : this->nodeList)
    {
        target.draw(node.second, state);
    }
}

void TrieVisualHandler::findStartX()
{
    int numLeaves = 0;
    for (const std::pair<int, TrieNode> &node : nodeList) {
        bool flag = 1;
        for (int i = 0; i < 26; i++) {
            if (node.second.ch[i] != -1) {
                flag = 0;
                break;
            }
        }
        if (flag) numLeaves++;
    }
    if (numLeaves == 0) {
        startX = centerLine;
        return;
    }
    float treeWidth = nodeRadius * 2 * numLeaves + padding * (numLeaves - 1);
    startX = centerLine - treeWidth / 2;
    return;
}

float TrieVisualHandler::recalPos(int id, int height)
{
    if (id == -1) return 0;
    // static int leafCnt = 0;
    std::cerr << "Recalculating pos: " << leafCnt << ' ' << id << ' ' << height << '\n';
    bool isLeaf = 1;
    TrieNode &curNode = nodeList[id];
    sf::Vector2f oldPos = curNode.getPos(), newPos;
    float maxX = INT_MIN, minX = INT_MAX;
    for (int i = 0; i < 26; i++) {
        // std::cerr << curNode.ch[i] << '\n';
        if (curNode.ch[i] != -1) {
            isLeaf = 0;
            float chX = recalPos(curNode.ch[i], height+1);
            // int chX = nodeList[curNode.ch[i]].getPos().x;
            maxX = std::max(maxX, chX + nodeRadius);
            minX = std::min(minX, chX - nodeRadius);
        }
    }
    if (isLeaf) {
        newPos = sf::Vector2f{startX + leafCnt * (nodeRadius * 2 + padding) + nodeRadius, startY + stepY * height};
        leafCnt++;
    }
    else {
        // std::cerr << maxX << ' ' << minX << ' ' << id << '\n';
        assert(maxX != INT_MIN && minX != INT_MAX);
        newPos = sf::Vector2f{(maxX + minX) / 2, startY + stepY * height};
    }

    // std::cerr << "Position of " << id << ": " << oldPos.x << ' ' << oldPos.y << ' ' << newPos.x << ' ' << newPos.y << '\n';

    Animation *tmp = new Animation();
    tmp->play(&curNode,oldPos,this->pos + newPos);
    animationList.push_back(tmp);

    return newPos.x;
}

TrieVisualHandler::TrieEdge TrieVisualHandler::calLinePosition(sf::Vector2f from, sf::Vector2f to)
{
    // Calculate the vector from the center to the edge in the direction pointing to each other
    sf::Vector2f v1 = this->nodeRadius * (to - from).normalized();
    sf::Vector2f v2 = this->nodeRadius * (from - to).normalized();

    return TrieEdge(from + v1, to + v2);
}

void TrieVisualHandler::recalLine()
{
    lineList.clear();
    for (std::pair<const int,TrieNode> &node : nodeList) {
        for (int i=0;i<26;i++) {
            int chId = node.second.ch[i];
            if (chId == -1) continue;

            lineList.push_back(calLinePosition(node.second.getPos(),
                nodeList[chId].getPos()));
            
            lineList.back().setHighlight(
                node.second.isHighlighted && nodeList[chId].isHighlighted
            );

            std::string tmp;
            tmp.push_back(char('a' + i));
            lineList.back().setString(tmp);
        }
    }
}

void TrieVisualHandler::setTreeStructure(const Global::TreeStructure &newStructure)
{
    std::cerr << "Trie - Visual: Structure reset\n";
    std::cerr << newStructure << '\n';

    this->root = newStructure.rootId;

    for (const std::pair<const int,Global::TrieChild_t> &newNode : newStructure.trieMap) {
        TrieNode &curNode = this->nodeList[newNode.first];
        curNode.isHighlighted = std::get<0>(newNode.second);
        curNode.setHighlighted(curNode.isHighlighted);
        curNode.setSpecial(std::get<1>(newNode.second));
        std::vector<std::pair<int,char>> chVec = std::get<2>(newNode.second);
        // std::cerr << newNode.first << '\n';
        for (int i = 0; i < 26; i++) curNode.ch[i] = -1;
        for (std::pair<int,char> ch : chVec) {
            // std::cerr << ch.first << ' ' << ch.second << ' ' << (int)ch.second - 'a' << '\n';
            curNode.ch[(int)ch.second - 'a'] = ch.first;
            this->nodeList[ch.first].paId = newNode.first;
        }
        curNode.setValue(newStructure.valueMap.at(newNode.first));
    }

    // Clean up
    for (auto it = nodeList.begin(); it != nodeList.end();) {
        if (newStructure.trieMap.find(it->first) == newStructure.trieMap.end()) {
            std::cerr << "Clean up: " << it->first << '\n';
            it = nodeList.erase(it);
        }
        else it++;
    }

    std::cerr << "Trie - Visual: Recalculating node position\n";
    leafCnt = 0;
    findStartX();
    recalPos(root,0);
    std::cerr << "Trie - Visual: Finish setting tree structure\n";
}

void TrieVisualHandler::updateAnimation()
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

void TrieVisualHandler::endAnimation()
{
    for (Animation *&ani : animationList) {
        ani->end();
    }
}

bool TrieVisualHandler::isAnimationEnd()
{
    return animationList.empty();
}

void TrieVisualHandler::clear()
{
    lineList.clear();
    nodeList.clear();
    curTreeHeight = 0;
}

//======================================================//

// Graph visual handler implementation

GraphVisualHandler::GraphVisualHandler(
    sf::Vector2f size,
    sf::Vector2f pos
)
{
    this->size = size;
    this->pos = pos;

    this->center = size * 0.5f;
}

void GraphVisualHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    for (int i = 0; i < lines.size(); i++) {
        target.draw(lines[i],state);
    }
    for (int i = 0; i < nodes.size(); i++) {
        target.draw(nodes[i],state);
    }
}

float fa(float dis, float k)
{
    return dis * dis / k;
}

float fr(float dis, float k)
{
    return k * k / dis;
}

float cool(float t)
{
    return t * 0.95;
}

void GraphVisualHandler::simulation(float forceConst)
{
    // const float targetDis = 50; // px
    const float dampening = 0.1;
    const float widthOffset = 80, heightOffset = 60;

    // Repulsion
    for (int u = 0; u < numNode; u++) {
        disp[u] = sf::Vector2f(0,0);
        for (int v = 0; v < numNode; v++) {
            if (u == v) continue;
            sf::Vector2f delta = nodes[u].getPos() - nodes[v].getPos();

            disp[u] += delta.normalized() * fr(delta.length(),forceConst);
        }
    }

    // Attraction
    for (Global::GraphStructure::Edge &e : baseStructure.edgeList) {
        int u = e.x, v = e.y;
        sf::Vector2f diff = nodes[v].getPos() - nodes[u].getPos();
        float force = fa(diff.length(),forceConst);
        
        diff = diff.normalized();
        disp[v] -= diff * force;
        disp[u] += diff * force;
    }

    for (int i = 0; i < numNode; i++) {
        // std::cerr << i << ' ' << disp[i].x << ' ' << disp[i].y << ' ' << disp[i].length() << '\n';
        disp[i] *= dampening;
        sf::Vector2f tmpPos = nodes[i].getPos();
        tmpPos += disp[i].normalized() * std::min(disp[i].length(), temperature);
        tmpPos.x = std::clamp(tmpPos.x, 0.f + widthOffset, size.x - widthOffset);
        tmpPos.y = std::clamp(tmpPos.y, 0.f + heightOffset, size.y - heightOffset);
        nodes[i].setPos(tmpPos);
    }
}

std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
int rng(int l,int r)
{
    return std::uniform_int_distribution<int>(l,r)(rnd);
}

void GraphVisualHandler::calPos()
{
    // float dt = physDuration / numTicks;
    disp.assign(numNode, {0,0});
    int iterNum = 40;
    float forceConst = 0.3 * sqrtf(1.f * (size.x * size.y) / nodes.size());
    temperature = 100;
    for (int i = 0; i < iterNum; i++) {
        simulation(forceConst);
    }
    // std::cerr << forceConst << '\n';
    std::cerr << "Simulation end\n";
}

void GraphVisualHandler::buildGraph(const Global::GraphStructure &structure)
{
    std::cerr << "Graph - Visual: Building graph\n";
    numNode = structure.numNode;
    for (int i = 0; i < numNode; i++) {
        nodes.emplace_back(
            sf::Vector2f{rng(0,size.x),rng(0,size.y)},std::to_string(i),nodeRadius,charSize,0,2,2,
            Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]
        );
        nodes[i].setHighlighted(false);
        nodes[i].setSpecial(false);
    }
    calPos();
    for (int i = 0; i < numNode; i++) {
        nodes[i].setPos(nodes[i].getPos() + pos);
    } 
    std::cerr << "Node position set\n";
    for (Global::GraphStructure::Edge edge : structure.edgeList)
    {
        int u = edge.x, v = edge.y, w = edge.w;
        lines.emplace_back(
            nodes[u].getPos(), nodes[v].getPos(), false,
            std::to_string(w), 2, 20
        );
        // lines.back().setSpecial(edge.isSpecial);
        // lines.back().setHighlight(edge.isHighlighted);
    }
    std::cerr << "Edge set\n";
    baseStructure = structure;
    
}

bool checkSimiliarStruct(const Global::GraphStructure &struct1, const Global::GraphStructure &struct2)
{
    // std::cerr << "Graph - Visual: Structure check begin\n";
    // std::cerr << struct1.edgeList.size() << ' ' << struct2.edgeList.size() << '\n';
    // for (int i = 0; i < struct1.edgeList.size(); i++) {
    //     std::cerr << struct1.edgeList[i].x << ' ' << struct1.edgeList[i].y << ' ' << struct1.edgeList[i].w << '\n';
    // }
    // std::cerr << '\n';
    // for (int i = 0; i < struct2.edgeList.size(); i++) {
    //     std::cerr << struct2.edgeList[i].x << ' ' << struct2.edgeList[i].y << ' ' << struct2.edgeList[i].w << '\n';
    // }
    if (struct1.edgeList.size() != struct2.edgeList.size()) return false;
    // std::cerr << "Same size\n";
    for (int i = 0; i < struct1.edgeList.size(); i++) {
        Global::GraphStructure::Edge e1 = struct1.edgeList[i], e2 = struct2.edgeList[i];
        if (e1.x != e2.x || e1.y != e2.y || e1.w != e2.w) {
            std::cerr << e1.x << ' ' << e1.y << ' ' << e1.w << ' '
                      << e2.x << ' ' << e2.y << ' ' << e2.w << '\n';
            return false;
        }
    }
    return true;
}

void GraphVisualHandler::setCurrentState(const Global::GraphStructure &newStructure)
{
    assert(checkSimiliarStruct(baseStructure,newStructure));
    assert(newStructure.edgeList.size() == lines.size());
    assert(newStructure.numNode == nodes.size());
    for (int i = 0; i < lines.size(); i++) {
        lines[i].setSpecial(newStructure.edgeList[i].isSpecial);
        lines[i].setHighlight(newStructure.edgeList[i].isHighlighted);
    }
    for (int i = 0; i < nodes.size(); i++) {
        nodes[i].setHighlighted(newStructure.nodeState[i].first);
        nodes[i].setSpecial(newStructure.nodeState[i].second);
    }
}

void GraphVisualHandler::clear()
{
    numNode = 0;
    nodes.clear();
    disp.clear();
    lines.clear();
    baseStructure = Global::GraphStructure();
}

//======================================================//

// Code visual handler implementation

CodeVisualHandler::CodeVisualHandler(
    sf::Vector2f pos,
    sf::Vector2f lineSize,
    int charSize,
    std::vector<std::string> funcNames,
    std::vector<std::string> filenames
)
{
    this->pos = pos;
    this->lineSize = lineSize;

    this->charSize = charSize;
    // this->maxLine = maxLine;
    // this->numFunc = numFunc;

    for (const std::string &file : filenames) {
        loadCode(file);
    }
    this->funcNames = funcNames;

    this->title = GUI::RectangleButton(
        {110,29}, {pos.x + lineSize.x - 110, pos.y - 29},
        "", 22, 0, 1,
        Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND],
        Global::colorSet[0][Global::COLOR_TYPE::MAIN],
        Global::colorSet[0][Global::COLOR_TYPE::NETURAL]
    );

    this->bg.setPosition(pos);
    this->bg.setOutlineThickness(1);
    this->bg.setOutlineColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
}

void CodeVisualHandler::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (curFunc < 0) return;
    target.draw(title,state);
    target.draw(bg,state);
    for (int i = 0; i < codeBg.size(); i++) target.draw(codeBg[i],state);
    for (int i = 0; i < codeTexts.size(); i++) target.draw(codeTexts[i],state);
    // std::cerr << codeTexts.size() << '\n';
}

void CodeVisualHandler::loadCode(const std::string &filename)
{
    std::cerr << "Loading code: " << filename << '\n';
    std::string s;
    std::ifstream inp(filename);
    std::vector<std::string> code;
    while (std::getline(inp,s)) {
        // std::cerr << s << '\n';
        code.push_back(s);
    }
    this->codes.push_back(code);
}

void CodeVisualHandler::generateVisual()
{
    if (curFunc < 0) return;
    codeBg.clear();
    codeTexts.clear();
    std::cerr << "Generating visual of: ";
    std::cerr << curFunc << ' ' << curLine << '\n';
    sf::Vector2f size;
    int numLine = codes[curFunc].size();
    for (int i = 0; i < numLine; i++) {
        sf::RectangleShape rect; 
        rect.setSize(lineSize);
        rect.setPosition({pos.x, pos.y + lineSize.y * i});
        codeBg.push_back(rect);

        sf::Text text(Global::codeFont);
        text.setFillColor(Global::colorSet[0][Global::COLOR_TYPE::NETURAL]);
        text.setCharacterSize(charSize);
        text.setString(codes[curFunc][i]);
        text.setOrigin({0, text.getLocalBounds().position.y + text.getLocalBounds().size.y * 0.5f});
        text.setPosition(rect.getPosition() + sf::Vector2f(10, int(lineSize.y * 0.5f)));
        // text.setPosition();
        codeTexts.push_back(text);

        // std::cerr << codes[curFunc][i] << ' ' << text.getPosition().x << ' ' << text.getPosition().y << '\n';
    }
    // std::cerr << codeTexts.size() << '\n';
    bg.setSize({lineSize.x, lineSize.y * numLine});
}

void CodeVisualHandler::highlightCur()
{
    assert(curFunc >= 0 && curLine >= 0);
    codeBg[curLine].setFillColor(Global::colorSet[0][Global::COLOR_TYPE::CODE_HIGHLIGHT]);
}

void CodeVisualHandler::unhighlightCur()
{
    assert(curFunc >= 0 && curLine >= 0);
    codeBg[curLine].setFillColor(Global::colorSet[0][Global::COLOR_TYPE::BACKGROUND]);
}

void CodeVisualHandler::setFunc(int func)
{
    std::cerr << "Code visual handler: " << func << ' ' << codes.size() << '\n';
    curFunc = func;
    if (func == -1) return; 
    // assert(func > -1);
    // assert(func < (int)codes.size());
    title.setString(funcNames[curFunc]);
    curLine = -1;
    generateVisual();
}

void CodeVisualHandler::setLine(int line)
{
    if (curFunc < 0) return;
    assert(line < (int)codes[curFunc].size());
    if (curLine >= 0) unhighlightCur();
    curLine = line;
    if (curLine >= 0) highlightCur();
}

//======================================================//

void PopupGroup::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    if (!isEnable) return;
    for (int i = 0; i < elements.size(); i++) {
        target.draw(*elements[i],state);
    }
}

void PopupGroup::handleEvent(const std::optional<sf::Event> &e) 
{
    if (!isEnable) return;
    for (int i = 0; i < iElements.size(); i++) {
        iElements[i]->handleEvent(e);
    }
}

bool PopupGroup::containPos(sf::Vector2f pos)
{
    return true;
}

void PopupGroup::setWindow(sf::RenderTarget *target_ptr)
{
    IInteractableElement::setWindow(target_ptr);
    for (int i = 0; i < iElements.size(); i++) {
        iElements[i]->setWindow(target_ptr);
    }
}

void PopupGroup::open()
{
    isEnable = true;
}

void PopupGroup::close()
{
    isEnable = false;
}

//======================================================//

// Zoom view implementation

ZoomView::ZoomView() {}

void ZoomView::setCenter(const sf::Vector2f& center)
{
    view.setCenter(center);
}

void ZoomView::setSize(const sf::Vector2f& size)
{
    view.setSize(size);
}

void ZoomView::setWindow(sf::RenderWindow *window)
{
    this->window = window;
    view = window->getDefaultView();
}

bool ZoomView::containPos(sf::Vector2f pos)
{
    sf::FloatRect rect(view.getCenter() - view.getSize() * 0.5f, view.getSize());
    return rect.contains(pos);
}

void ZoomView::handleEvent(const std::optional<sf::Event>& e)
{
    if (const sf::Event::MouseWheelScrolled* scroll = e->getIf<sf::Event::MouseWheelScrolled>()) {
        sf::Vector2i pixelPos = scroll->position;
        std::cerr << "Scroll event registered\n";
        if (!containPos(sf::Vector2f(pixelPos))) return;
        std::cerr << "Position in\n";
        sf::Vector2f beforeZoom = window->mapPixelToCoords(pixelPos, view);
        if (scroll->delta > 0)
            view.zoom(1.f / zoomFactor);
        else
            view.zoom(zoomFactor);

        sf::Vector2f afterZoom = window->mapPixelToCoords(pixelPos, view);
        view.move(beforeZoom - afterZoom);
    }
    else if (const sf::Event::MouseButtonPressed* pressed = e->getIf<sf::Event::MouseButtonPressed>()) {
        if (!containPos(sf::Vector2f(pressed->position))) return;
        if (pressed->button == sf::Mouse::Button::Middle) {
            dragging = true;
            lastMousePos = pressed->position;
        }
    }
    else if (const sf::Event::MouseButtonReleased* released = e->getIf<sf::Event::MouseButtonReleased>()) {
        // if (!containPos(sf::Vector2f(released->position))) return;
        if (released->button == sf::Mouse::Button::Middle) {
            dragging = false;
        }
    }
    else if (const sf::Event::MouseMoved* moved = e->getIf<sf::Event::MouseMoved>()) {
        if (!dragging) return;
        sf::Vector2i currentPos = moved->position;
        sf::Vector2f delta = window->mapPixelToCoords(lastMousePos, view) - window->mapPixelToCoords(currentPos, view);
        view.move(delta);
        lastMousePos = currentPos;
    }
}

}