#include "../../include/data-structure/data-structure.h"

namespace DataStructure
{

//======================================================//

// Singly linked list implementation

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
    if (cur->val == x) return;
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

void Trie::clearRecur(Node *cur)
{
    for (int i=0;i<CHAR_NUM;i++) {
        if (cur->ch[i]) clearRecur(cur->ch[i]);
    }
    if (cur) delete cur;
}
bool Trie::removeRecur(std::string &s, int id, Node *cur)
{
    if (!cur) return;
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
    
}

}