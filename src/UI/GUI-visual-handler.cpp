#include "../../include/UI/GUI-visual-handler.h"

#include <math.h>
#include <fstream>

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
            if (node.second.isHighlighted && nodeList[node.second.leftCh].isHighlighted) {
                lineList.back().highlight();
            }
        }
        
        if (node.second.rightCh != -1) {
            lineList.push_back(calLinePosition(node.second.getPos(),
                nodeList[node.second.rightCh].getPos()));
            if (node.second.isHighlighted && nodeList[node.second.rightCh].isHighlighted) {
                lineList.back().highlight();
            }
        }
    }
}

void TreeVisualHandler::setTreeStructure(Global::TreeStructure &newStructure)
{
    std::cerr << "Tree structure reset\n";
    // std::cerr << newStructure << '\n';
    this->root = newStructure.rootId;

    // Insert and update
    for (std::pair<const int,std::tuple<int,int,bool>> &newNode : newStructure.structureMap) {
        TreeNode &curNode = this->nodeList[newNode.first];
        curNode.leftCh = std::get<0>(newNode.second);
        curNode.rightCh = std::get<1>(newNode.second);
        curNode.setHighlighted(std::get<2>(newNode.second));
        curNode.isHighlighted = std::get<2>(newNode.second);
        if (curNode.leftCh != -1) this->nodeList[curNode.leftCh].paId = newNode.first;
        if (curNode.rightCh != -1) this->nodeList[curNode.rightCh].paId = newNode.first;
        
        // std::cerr << newNode.first << ' ' << newStructure.valueMap[newNode.first] << '\n';

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
            if (node.second.isHighlighted && nodeList[node.second.nxt].isHighlighted) {
                lineList.back().highlight();
            }
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

void SLLVisualHandler::setTreeStructure(Global::TreeStructure &newStructure)
{
    std::cerr << "Tree structure reset\n";
    this->root = newStructure.rootId;

    for (std::pair<const int,std::tuple<int,int,bool>> &newNode : newStructure.structureMap) {
        StructNode &curNode = this->nodeList[newNode.first];
        curNode.nxt = std::get<0>(newNode.second);
        curNode.setHighlighted(std::get<2>(newNode.second));
        curNode.isHighlighted = std::get<2>(newNode.second);
        if (curNode.nxt != -1) this->nodeList[curNode.nxt].prevId = newNode.first;

        curNode.setValue(newStructure.valueMap[newNode.first]);
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
            if (node.second.isHighlighted && nodeList[chId].isHighlighted) {
                lineList.back().highlight();
            }

            std::string tmp;
            tmp.push_back(char('a' + i));
            lineList.back().setString(tmp);
        }
    }
}

void TrieVisualHandler::setTreeStructure(Global::TreeStructure &newStructure)
{
    std::cerr << "Trie structure reset\n";

    this->root = newStructure.rootId;

    for (std::pair<const int,Global::TrieChild_t> &newNode : newStructure.trieMap) {
        TrieNode &curNode = this->nodeList[newNode.first];
        curNode.isHighlighted = std::get<0>(newNode.second);
        curNode.setSpecial(std::get<1>(newNode.second));
        std::vector<std::pair<int,char>> chVec = std::get<2>(newNode.second);
        for (std::pair<int,char> ch : chVec) {
            curNode.ch[ch.second - 'a'] = ch.first;
            this->nodeList[ch.first].paId = newNode.first;
        }
    }

    // Clean up
    for (auto it = nodeList.begin(); it != nodeList.end();) {
        if (newStructure.structureMap.find(it->first) == newStructure.structureMap.end()) {
            it = nodeList.erase(it);
        }
        else it++;
    }

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


}