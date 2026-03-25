#include "../../include/data-structure/data-structure.h"

#include <queue>
#include <iostream> // For debug

namespace DataStructure
{

void BaseStructure::linkSnapshot(std::vector<Global::TreeStructure> *snapshot_ptr)
{
    this->snapshot_ptr = snapshot_ptr;
}

void BaseStructure::clearSnapshot()
{
    if (!snapshot_ptr) return;
    snapshot_ptr->clear();
}

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

bool SinglyLinkedList::remove(int x)
{
    if (!head) return false;
    if (head->val == x) {
        Node *tmp = head;
        head = head->pNext;
        delete tmp;
        if (!head) tail = nullptr;
        return true;
    }
    for (Node *ptr = head; ptr->pNext; ptr = ptr->pNext) {
        if (ptr->pNext->val = x) {
            Node *tmp = ptr->pNext;
            ptr->pNext = ptr->pNext->pNext;
            delete tmp;
            if (tail == tmp) tail = ptr;
            return true;
        }
    }
    return false;
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
    if (minId == id) return;
    std::swap(vec[minId],vec[id]);
    heapify(minId);
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

void AVLTree::createSnapshotRecur(Node *cur, Global::TreeStructure &structure)
{
    if (!cur) return;
    // std::cerr << "Recur called\n";
    structure.valueMap[cur->id] = std::to_string(cur->val);
    int ltId = cur->ltCh ? cur->ltCh->id : -1;
    int rtId = cur->rtCh ? cur->rtCh->id : -1;
    structure.structureMap[cur->id] = std::make_tuple(ltId,rtId,cur->ishighlighted);
    
    createSnapshotRecur(cur->ltCh, structure);
    createSnapshotRecur(cur->rtCh, structure);
}

void AVLTree::createSnapshot()
{
    snapshot_ptr->push_back(Global::TreeStructure());
    createSnapshotRecur(root, snapshot_ptr->back());
    snapshot_ptr->back().rootId = root ? root->id : -1;

    std::cerr << "AVL snapshot created\n";
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

void AVLTree::leftRotate(Node *&cur)
{
    if (!cur->rtCh) return;
    Node *res = cur->rtCh;
    Node *tmp = cur->rtCh->ltCh;

    cur->rtCh->ltCh = cur;
    cur->rtCh = tmp;

    recalHeight(cur);
    recalHeight(res);

    cur = res;

    createSnapshot();
}
void AVLTree::rightRotate(Node *&cur)
{
    if (!cur->ltCh) return;
    Node *res = cur->ltCh;
    Node *tmp = cur->ltCh->rtCh;

    cur->ltCh->rtCh = cur;
    cur->ltCh = tmp;

    recalHeight(cur);
    recalHeight(res);

    cur = res;

    createSnapshot();

    return;
}

void AVLTree::balancing(Node *&cur)
{
    recalHeight(cur);
    int ltH = cur->leftHeight(), rtH = cur->rightHeight();
    std::cerr << "Balancing: " << cur->id << ' ' << ltH << ' ' << rtH << '\n';
    if (ltH - rtH >= -1 && ltH - rtH <= 1) return;

    std::cerr << "Balancing called \n";

    if (ltH > rtH) {
        std::cerr << "Left unbalanced\n";
        if (cur->ltCh->leftHeight() <= cur->ltCh->rightHeight()) 
            leftRotate(cur->ltCh);
        return rightRotate(cur);
    }
    else {
        std::cerr << "Right unbalanced" << '\n';
        if (cur->rtCh->leftHeight() >= cur->rtCh->rightHeight()) 
            rightRotate(cur->rtCh);
        return leftRotate(cur);
    }
}
void AVLTree::findMax(Node *&cur, int& retVal)
{
    if (cur->rtCh) {
        findMax(cur->rtCh, retVal);
        return;
    }
    retVal = cur->val;
    Node *tmp = cur->ltCh;
    delete cur;
    cur = tmp;
    return;
}

void AVLTree::insertRecur(int x,Node *&cur)
{
    std::cerr << x << '\n';
    if (!cur) {
        cur = new Node();
        cur->id = counter;
        cur->val = x;

        createSnapshot();
        return;
    }
    std::cerr << "Current id: " << cur->id << '\n';

    cur->ishighlighted = true;
    createSnapshot();
    
    if (cur->val == x) {
        counter--;
        cur->ishighlighted = false;
        createSnapshot();
        return;
    }

    if (x < cur->val) {
        if (!cur->ltCh) {
            cur->ltCh = new Node();
            cur->ltCh->id = counter;
            cur->ltCh->val = x;
            
            createSnapshot();
        }
        else {
            insertRecur(x,cur->ltCh);
        }
    }
    else {
        if (!cur->rtCh) {
            cur->rtCh = new Node();
            cur->rtCh->id = counter;
            cur->rtCh->val = x;

            createSnapshot();
        }
        else {
            insertRecur(x,cur->rtCh);
        }
    }
    cur->ishighlighted = false;
    balancing(cur);
    createSnapshot();
    std::cerr << "Insert recur " << cur->id << " end\n";
    return;
}

void AVLTree::removeRecur(int x,Node *&cur)
{
    if (x < cur->val) {
        removeRecur(x, cur->ltCh);
    }
    else if (x > cur->val) {
        removeRecur(x,cur->rtCh);
    }
    else {
        if (!cur->ltCh) {
            Node *tmp = cur->rtCh;
            delete cur;
            cur = tmp;
            return;
        }
        else if (!cur->rtCh) {
            Node *tmp = cur->ltCh;
            delete cur;
            cur = tmp;
            return;
        }
        else {
            int tmpVal = INT_MIN;
            findMax(cur->ltCh,tmpVal);
            cur->val = tmpVal;
        }
    }

    balancing(cur);
}

void AVLTree::clearRecur(Node *cur) {
    if (!cur) return;
    if (cur->ltCh) clearRecur(cur->ltCh);
    if (cur->rtCh) clearRecur(cur->rtCh);
    delete cur;
}

void AVLTree::insert(int x)
{
    std::cerr << "AVL insert called\n";
    counter++;
    insertRecur(x,root);
    std::cerr << "AVL insert end\n";
}
bool AVLTree::find(int x)
{
    Node *cur = root;
    std::cerr << "Find begin " << x << '\n';
    while (cur) {
        std::cerr << cur->val << '\n';
        if (cur->val == x) return true;
        if (x < cur->val) cur = cur->ltCh;
        else cur = cur->rtCh;
    }
    std::cerr << "Find end\n";
    return false;
}
bool AVLTree::remove(int x)
{
    if (find(x)) {
        removeRecur(x,root);
        return true;
    }
    return false;
}
void AVLTree::clear()
{
    clearRecur(root);
    root = nullptr;
    counter = 0;
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