#include "../../include/data-structure/data-structure.h"

#include <queue>

namespace DataStructure
{

//======================================================//

// Singly linked list implementation

SinglyLinkedList::~SinglyLinkedList()
{
    this->clear();
}

void SinglyLinkedList::insert(int x)
{
    counter++;
    if (!head) {
        head = new Node();
        head->id = counter;
        head->val = x;
        tail = head;
        return;
    }
    tail->pNext = new Node();
    tail = tail->pNext;
    tail->id = counter;
    tail->val = x;
}

bool SinglyLinkedList::find(int x)
{
    for (Node *ptr = head; ptr; ptr = ptr->pNext) {
        if (ptr->val == x) return true;
    }
    return false;
}

void SinglyLinkedList::remove(int x)
{
    if (!head) return;
    if (head->val == x) {
        Node *tmp = head;
        head = head->pNext;
        delete tmp;
        if (!head) tail = nullptr;
        return;
    }
    for (Node *ptr = head; ptr->pNext; ptr = ptr->pNext) {
        if (ptr->pNext->val = x) {
            Node *tmp = ptr->pNext;
            ptr->pNext = ptr->pNext->pNext;
            delete tmp;
            if (tail == tmp) tail = ptr;
        }
    }
}

void SinglyLinkedList::clear()
{
    while (!head) {
        Node *tmp = head;
        head = head->pNext;
        delete tmp;
    }
    tail = nullptr;
    counter = 0;
}

//======================================================//

// Heap implementation (using vector)

Heap::~Heap()
{
    this->clear();
}

int Heap::findPa(int x) {
    return (x-1) / 2;
}
int Heap::findLt(int x) {
    return x * 2 + 1;
}
int Heap::findRt(int x) {
    return x * 2 + 2;
}

void Heap::heapify(int id) {
    int lt = findLt(id);
    int rt = findRt(id);
    int minId = id;
    if (lt < vec.size() && vec[lt] > vec[id]) minId = lt;
    if (rt < vec.size() && vec[rt] > vec[id]) minId = rt;
}

void Heap::insert(int x)
{
    counter++;
    int id = vec.size();
    vec.push_back(x);

}

std::optional<int> Heap::getMax()
{
    std::optional<int> res;
    if (vec.size()) res = vec[0];
    return res;
}

void Heap::removeMax()
{
    if (vec.empty()) return;
    std::swap(vec[0],vec.back());
    vec.pop_back();
    heapify(0);
}

void Heap::clear()
{
    vec.clear();
    counter = 0;
}

//======================================================//

// AVL implementation

AVLTree::~AVLTree()
{
    this->clear();
}

int AVLTree::Node::leftHeight() 
{
    return ltCh ? ltCh->height : 0;
}
int AVLTree::Node::rightHeight()
{
    return rtCh ? rtCh->height : 0;
}
void AVLTree::recalHeight(Node *cur) 
{
    cur->height = std::max(cur->leftHeight(),cur->rightHeight()) + 1;
}

AVLTree::Node* AVLTree::leftRotate(Node *cur)
{
    if (!cur->rtCh) return cur;
    Node *res = cur->rtCh;
    Node *tmp = cur->rtCh->ltCh;

    cur->rtCh->ltCh = cur;
    cur->rtCh = tmp;

    recalHeight(cur);
    recalHeight(res);

    return res;
}
AVLTree::Node* AVLTree::rightRotate(Node *cur)
{
    if (!cur->ltCh) return cur;
    Node *res = cur->ltCh;
    Node *tmp = cur->ltCh->rtCh;

    cur->ltCh->rtCh = cur;
    cur->ltCh = tmp;

    recalHeight(cur);
    recalHeight(res);

    return res;
}

AVLTree::Node* AVLTree::balancing(Node *cur)
{
    recalHeight(cur);
    int ltH = cur->leftHeight(), rtH = cur->rightHeight();
    if (ltH - rtH >= -1 && ltH - rtH <= 1) return cur;
    if (ltH > rtH) {
        if (cur->ltCh->leftHeight() <= cur->rtCh->rightHeight()) 
            cur->ltCh = leftRotate(cur->ltCh);
        return rightRotate(cur);
    }
    else {
        if (cur->ltCh->leftHeight() >= cur->rtCh->rightHeight()) 
            cur->rtCh = rightRotate(cur->rtCh);
        return leftRotate(cur);
    }
}
AVLTree::Node* AVLTree::findMax(Node *cur, int& retVal)
{
    if (cur->rtCh) {
        cur->rtCh = findMax(cur->rtCh, retVal);
        return cur;
    }
    retVal = cur->val;
    Node *tmp = cur->rtCh;
    delete cur;
    return tmp;
}

AVLTree::Node* AVLTree::insertRecur(int x,Node *cur)
{
    if (cur->val == x) return cur;
    if (cur->val < x) {
        if (!cur->ltCh) {
            cur->ltCh = new Node();
            cur->id = counter;
            cur->val = x;
        }
        else {
            cur->ltCh = insertRecur(x,cur->ltCh);
        }
    }
    else {
        if (!cur->rtCh) {
            cur->rtCh = new Node();
            cur->id = counter;
            cur->val = x;
        }
        else {
            cur->rtCh = insertRecur(x,cur->rtCh);
        }
    }
    return balancing(cur);
}

AVLTree::Node* AVLTree::removeRecur(int x,Node *cur)
{
    if (cur->val < x) {
        cur->ltCh = removeRecur(x, cur->ltCh);
    }
    else if (cur->val > x) {
        cur->rtCh = removeRecur(x,cur->rtCh);
    }
    else {
        if (!cur->ltCh) {
            Node *tmp = cur->rtCh;
            delete cur;
            return tmp;
        }
        else if (!cur->rtCh) {
            Node *tmp = cur->ltCh;
            delete cur;
            return tmp;
        }
        else {
            int tmpVal = INT_MIN;
            cur->ltCh = findMax(cur->ltCh,tmpVal);
            cur->val = tmpVal;
        }
    }

    return balancing(cur);
}

void AVLTree::clearRecur(Node *cur) {
    if (cur->ltCh) clearRecur(cur->ltCh);
    if (cur->rtCh) clearRecur(cur->rtCh);
    if (cur) delete cur;
}

void AVLTree::insert(int x)
{
    counter++;
    root = insertRecur(x,root);
}
bool AVLTree::find(int x)
{
    Node *cur = root;
    while (cur) {
        if (cur->val == x) return true;
        if (cur->val < x) cur = cur->ltCh;
        else cur = cur->rtCh;
    }
    return false;
}
void AVLTree::remove(int x)
{
    root = removeRecur(x,root);
}
void AVLTree::clear()
{
    clearRecur(root);
    root = nullptr;
    counter = 0;
}

//======================================================//

// Trie implementation

Trie::~Trie()
{
    this->clear();
}

void Trie::clearRecur(Node *cur)
{
    for (int i=0;i<CHAR_NUM;i++) {
        if (cur->ch[i]) clearRecur(cur->ch[i]);
    }
    if (cur) delete cur;
}
bool Trie::removeRecur(std::string &s, int id, Node *cur)
{
    if (!cur) return false;
    int c = s[id] - 'a';
    if (!cur->ch[c]) return false;
    if (removeRecur(s,id+1,cur->ch[c])) {
        cur->cnt--;
        if (!cur->cnt) delete cur;
        return true;
    }
    return false;
}

void Trie::insert(std::string s)
{
    if (!root) {
        root = new Node();
        root->id = ++counter;
    }
    Node *cur = root;
    cur->cnt++;
    for (int i=0;i<s.size();i++) {
        int c = s[i] - 'a';
        if (!cur->ch[c]) {
            cur->ch[c] = new Node();
            cur->ch[c]->id = ++counter;
        }
        cur = cur->ch[i];
        cur->cnt++;
    }
    cur->exist++;
}

bool Trie::find(std::string s)
{
    if (!root) return false;
    Node *cur = root;
    for (int i=0;i<s.size();i++) {
        int c = s[i] - 'a';
        if (!cur->ch[c]) return false;
        cur = cur->ch[c];
    }
    return cur->exist != 0;
}

void Trie::remove(std::string s)
{
    removeRecur(s,0,root);
}

void Trie::clear()
{
    clearRecur(root);
}

//======================================================//

// Graph implementation

Graph::~Graph()
{
    this->clear();
}

void Graph::setSize(int n)
{
    this->n = n;
    adj.assign(n,std::vector<std::pair<int,int>>());
}

void Graph::addEdge(int x,int y,int w)
{
    adj[x].push_back({y,w});
    adj[y].push_back({x,w});
}

int Graph::findMST()
{
    if (this->n == 0) return -1;
    
    std::priority_queue<std::pair<int,int>> pq;
    std::vector<bool> vst(this->n);

    pq.push({0,0});
    vst[0] = 1;
    int cnt = 1, res = 0;

    while (pq.size()) {
        int u = pq.top().second;
        int curW = -pq.top().first; 
        pq.pop();
        if (vst[u]) continue;
        cnt++;
        vst[u] = 1;
        res += curW;
        if (cnt == this->n) break;
        for (std::pair<int,int> &edge : adj[u])
        {
            int &v = edge.first, &w = edge.second;
            if (vst[v]) continue;
            pq.push({-w,v});
        }
    }

    return res;
}

int Graph::findSP(int st,int ed)
{
    if (this->n == 0) return -1;
    if (st >= this->n || ed >= this->n) return -1;
    
    std::priority_queue<std::pair<int,int>> pq;
    std::vector<int> dis(this->n,-1);

    pq.push({0,st});
    dis[st] = 0;
    int cnt = 1, res = 0;

    while (pq.size()) {
        int u = pq.top().second;
        int curDis = -pq.top().first; 
        pq.pop();
        if (u == ed) return dis[ed];
        if (dis[u] != -1) continue;
        cnt++;
        if (cnt == n) break;
        for (std::pair<int,int> &edge : adj[u])
        {
            int &v = edge.first, &w = edge.second;
            if (dis[v] != -1) continue;
            dis[v] = curDis + w;
            pq.push({-dis[v],v});
        }
    }
    return -1;
}

void Graph::clear()
{
    this->n = 0;
    adj.clear();
}

}