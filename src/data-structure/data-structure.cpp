#include "../../include/data-structure/data-structure.h"

#include <queue>
#include <stack>
#include <iostream> // For debug

using Global::toInt;

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

void SinglyLinkedList::createSnapshot(int func, int line)
{
    snapshot_ptr->push_back(Global::TreeStructure());
    Global::TreeStructure &snp = snapshot_ptr->back();
    snp.rootId = this->head ? this->head->id : -1;
    for (Node *cur = this->head; cur; cur = cur->pNext) {
        snp.valueMap[cur->id] = std::to_string(cur->val);
        snp.structureMap[cur->id] = std::make_tuple(cur->pNext ? cur->pNext->id : -1, -1, cur->ishighlighted);
    }
    snp.codeFunction = func;
    snp.codeLine = line;
}

void SinglyLinkedList::insert(int x)
{
    counter++;
    if (!head) {
        createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 0);
        head = new Node();
        head->id = counter;
        head->val = x;
        head->ishighlighted = true;
        createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 1);
        tail = head;
        head->ishighlighted = false;
        createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 2);
        // std::cerr << "Inser head null: " << x << ' ' << head << '\n';
        return;
    }
    createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 3);
    tail->pNext = new Node();
    tail = tail->pNext;
    tail->id = counter;
    tail->val = x;
    tail->ishighlighted = true;
    createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 4);
    tail->ishighlighted = false;
    createSnapshot(Global::toInt(Global::SLL_FUNC::INSERT), 5);
}

bool SinglyLinkedList::find(int x)
{
    for (Node *ptr = head; ptr; ptr = ptr->pNext) {
        ptr->ishighlighted = true;
        createSnapshot(Global::toInt(Global::SLL_FUNC::FIND), 0);
        createSnapshot(Global::toInt(Global::SLL_FUNC::FIND), 1);
        if (ptr->val == x) {
            createSnapshot(Global::toInt(Global::SLL_FUNC::FIND), 2);
            return true;
        }
        ptr->ishighlighted = false;
    }
    createSnapshot(Global::toInt(Global::SLL_FUNC::FIND), 3);
    return false;
}

bool SinglyLinkedList::remove(int x)
{
    // std::cerr << "======================== Remove called: " << x << '\n';
    if (!head) {
        createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 0);
        return false;
    }
    if (head->val == x) {
        head->ishighlighted = true;
        createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 1);
        head->ishighlighted = false;
        Node *tmp = head;
        head = head->pNext;
        delete tmp;
        createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 2);
        if (!head) {
            createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 3);
            tail = nullptr;
        }
        return true;
    }
    for (Node *ptr = head; ptr->pNext; ptr = ptr->pNext) {
        ptr->pNext->ishighlighted = true;
        ptr->ishighlighted = true;
        createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 5);
        createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 6);
        if (ptr->pNext->val == x) {
            Node *tmp = ptr->pNext;
            ptr->pNext = ptr->pNext->pNext;
            delete tmp;
            if (ptr->pNext) ptr->pNext->ishighlighted = true;
            createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 7);
            createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 8);
            
            // std::cerr << "Inside called\n";

            if (tail == tmp) {
                createSnapshot(Global::toInt(Global::SLL_FUNC::REMOVE), 9);
                tail = ptr;
            }
            if (ptr->pNext) ptr->pNext->ishighlighted = false;
            ptr->ishighlighted = false;
            return true;
        }
        ptr->pNext->ishighlighted = false;
        ptr->ishighlighted = false;
    }
    return false;
}

bool SinglyLinkedList::update(int x,int newVal)
{
    for (Node *cur = head; cur; cur = cur->pNext) {
        cur->ishighlighted = true;
        createSnapshot(Global::toInt(Global::SLL_FUNC::UPDATE), 0);
        createSnapshot(Global::toInt(Global::SLL_FUNC::UPDATE), 1);
        if (cur->val == x) {
            cur->val = newVal;
            createSnapshot(Global::toInt(Global::SLL_FUNC::UPDATE), 2);
            cur->ishighlighted = false;
            return true;
        }
        cur->ishighlighted = false;
    }
    return false;
}

void SinglyLinkedList::clear()
{
    // std::cerr << "SLL clear called\n";
    while (head) {
        // std::cerr << "SLL clear inside";
        Node *tmp = head;
        head = head->pNext;
        delete tmp;
    }
    tail = nullptr;
    // std::cerr << "SLL clear ended\n";
    counter = 0;
}

//======================================================//

// Heap implementation (using vector)

Heap::~Heap()
{
    this->clear();
}

void Heap::createSnapshot(int func,int line)
{
    // std::cerr << "Heap snapshot: " << func << ' ' << line << '\n';
    snapshot_ptr->push_back(Global::TreeStructure());
    Global::TreeStructure &tmp = snapshot_ptr->back();
    tmp.codeFunction = func;
    tmp.codeLine = line;
    if (vec.empty()) {
        tmp.rootId = -1;
        return;
    }
    tmp.rootId = vec[0].id;
    for (int i = 0; i < vec.size(); i++) {
        tmp.valueMap[vec[i].id] = std::to_string(vec[i].val);
        int ltId = findLt(i);
        ltId = ltId >= vec.size() ? -1 : vec[ltId].id;
        int rtId = findRt(i);
        rtId = rtId >= vec.size() ? -1 : vec[rtId].id;
        tmp.structureMap[vec[i].id] = std::make_tuple(ltId,rtId,vec[i].isHighlighted);
    }
}

int Heap::findPa(int x)
{
    return (x-1) / 2;
}
int Heap::findLt(int x)
{
    return x * 2 + 1;
}
int Heap::findRt(int x)
{
    return x * 2 + 2;
}

void Heap::downheap(int id)
{
    int lt = findLt(id);
    int rt = findRt(id);

    vec[id].isHighlighted = true;
    createSnapshot(toInt(Global::HEAP_FUNC::DOWNHEAP),0);

    int minId = id;
    if (lt < vec.size() && vec[lt].val > vec[minId].val) minId = lt;
    if (rt < vec.size() && vec[rt].val > vec[minId].val) minId = rt;
    
    if (minId == id) {
        vec[id].isHighlighted = false;
        createSnapshot(toInt(Global::HEAP_FUNC::DOWNHEAP),1);
        return;
    }

    vec[minId].isHighlighted = true;
    createSnapshot(toInt(Global::HEAP_FUNC::DOWNHEAP),2);

    std::swap(vec[minId],vec[id]);
    createSnapshot(toInt(Global::HEAP_FUNC::DOWNHEAP),2);

    vec[id].isHighlighted = false;
    createSnapshot(toInt(Global::HEAP_FUNC::DOWNHEAP),3);

    downheap(minId);
    // vec[id].isHighlighted = false;
}

void Heap::upheap(int id)
{
    while (true) {
        vec[id].isHighlighted = true;
        createSnapshot(toInt(Global::HEAP_FUNC::UPHEAP),0);
        if (id != 0 && vec[findPa(id)].val < vec[id].val) {
            vec[findPa(id)].isHighlighted = true;
            createSnapshot(toInt(Global::HEAP_FUNC::UPHEAP),1);

            std::swap(vec[findPa(id)], vec[id]);
            createSnapshot(toInt(Global::HEAP_FUNC::UPHEAP),1);

            vec[id].isHighlighted = false;
            id = findPa(id);
            createSnapshot(toInt(Global::HEAP_FUNC::UPHEAP),2);
        }
        else break;
    }
    vec[id].isHighlighted = false;
    createSnapshot(toInt(Global::HEAP_FUNC::UPHEAP),-1);
}

bool Heap::checkId(int id)
{
    return id < vec.size();
}

void Heap::insert(int x)
{
    counter++;
    int id = vec.size();
    createSnapshot(toInt(Global::HEAP_FUNC::INSERT),0);
    vec.emplace_back(counter,x);
    vec.back().isHighlighted = true;
    createSnapshot(toInt(Global::HEAP_FUNC::INSERT),1);
    createSnapshot(toInt(Global::HEAP_FUNC::INSERT),2);
    upheap(id);
}

int Heap::getMax()
{
    if (vec.size()) {
        return vec[0].val;
    }
    return -1;
}

void Heap::pop()
{
    if (vec.empty()) {
        createSnapshot(toInt(Global::HEAP_FUNC::POP),0);
        return;
    }
    std::swap(vec[0],vec.back());
    createSnapshot(toInt(Global::HEAP_FUNC::POP),1);
    
    vec.pop_back();
    createSnapshot(toInt(Global::HEAP_FUNC::POP),2);
    if (vec.empty()) return;
    createSnapshot(toInt(Global::HEAP_FUNC::POP),3);
    downheap(0);
}

void Heap::updateById(int id,int newVal)
{
    // if (id >= vec.size()) return;
    int preVal = vec[id].val;
    vec[id].isHighlighted = true;
    createSnapshot(toInt(Global::HEAP_FUNC::UPDATE_BY_ID),0);
    vec[id].val = newVal;
    createSnapshot(toInt(Global::HEAP_FUNC::UPDATE_BY_ID),1);
    if (preVal > newVal) {
        createSnapshot(toInt(Global::HEAP_FUNC::UPDATE_BY_ID),2);
        downheap(id);
    }
    else {
        createSnapshot(toInt(Global::HEAP_FUNC::UPDATE_BY_ID),3);
        upheap(id);
    }
}

void Heap::removeById(int id)
{

    while (true) {
        vec[id].isHighlighted = true;
        createSnapshot(toInt(Global::HEAP_FUNC::REMOVE_BY_ID),0);
        if (id == 0) {
            vec[id].isHighlighted = false;
            break;
        }
        vec[findPa(id)].isHighlighted = true;
        createSnapshot(toInt(Global::HEAP_FUNC::REMOVE_BY_ID),1);

        std::swap(vec[findPa(id)],vec[id]);
        createSnapshot(toInt(Global::HEAP_FUNC::REMOVE_BY_ID),1);

        vec[id].isHighlighted = false;
        id = findPa(id);
        createSnapshot(toInt(Global::HEAP_FUNC::REMOVE_BY_ID),2);
    }
    createSnapshot(toInt(Global::HEAP_FUNC::REMOVE_BY_ID),3);
    Heap::pop();
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

void AVLTree::createSnapshot(int func, int line)
{
    snapshot_ptr->push_back(Global::TreeStructure());
    createSnapshotRecur(root, snapshot_ptr->back());
    snapshot_ptr->back().rootId = root ? root->id : -1;
    snapshot_ptr->back().codeFunction = func;
    snapshot_ptr->back().codeLine = line;

    // std::cerr << "AVL snapshot created\n";
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

    // createSnapshot(toInt(Global::AVL_FUNC::BALANCE), );
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

    // createSnapshot();
}

void AVLTree::balancing(Node *&cur)
{
    recalHeight(cur);
    int ltH = cur->leftHeight(), rtH = cur->rightHeight();
    // std::cerr << "Balancing: " << cur->id << ' ' << ltH << ' ' << rtH << '\n';
    createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 0);
    if (ltH - rtH >= -1 && ltH - rtH <= 1) {
        createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 5);
        return;
    }

    // std::cerr << "Balancing called \n";

    if (ltH > rtH) {
        // std::cerr << "Left unbalanced\n";
        if (cur->ltCh->leftHeight() <= cur->ltCh->rightHeight()) {
            leftRotate(cur->ltCh);
            createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 1);
            rightRotate(cur);
            createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 1);
            return;
        }
        rightRotate(cur);
        createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 2);
    }
    else {
        // std::cerr << "Right unbalanced" << '\n';
        if (cur->rtCh->leftHeight() >= cur->rtCh->rightHeight()) {
            rightRotate(cur->rtCh);
            createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 3);
            leftRotate(cur);
            createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 3);
            return;
        }
        leftRotate(cur);
        createSnapshot(toInt(Global::AVL_FUNC::BALANCE), 4);
    }
}
void AVLTree::findMax(Node *&cur, int& retVal)
{
    cur->ishighlighted = true;
    createSnapshot(toInt(Global::AVL_FUNC::FIND), 0);
    if (cur->rtCh) {
        findMax(cur->rtCh, retVal);
        cur->ishighlighted = false;
        createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 1);
        return;
    }
    cur->ishighlighted = false;
    retVal = cur->val;
    Node *tmp = cur->ltCh;
    delete cur;
    cur = tmp;

    createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 1);

    return;
}

void AVLTree::insertRecur(int x,Node *&cur)
{
    // std::cerr << x << '\n';
    if (!cur) {
        cur = new Node();
        cur->id = counter;
        cur->val = x;

        createSnapshot(toInt(Global::AVL_FUNC::INSERT), 0);
        return;
    }
    // std::cerr << "Current id: " << cur->id << '\n';

    cur->ishighlighted = true;
    createSnapshot(toInt(Global::AVL_FUNC::INSERT), 0);
    
    if (cur->val == x) {
        counter--;
        cur->ishighlighted = false;
        createSnapshot(toInt(Global::AVL_FUNC::INSERT), 1);
        return;
    }

    if (x < cur->val) {
        createSnapshot(toInt(Global::AVL_FUNC::INSERT), 3);
        insertRecur(x,cur->ltCh);
    }
    else {
        createSnapshot(toInt(Global::AVL_FUNC::INSERT), 2);
        insertRecur(x,cur->rtCh);
    }
    cur->ishighlighted = false;
    createSnapshot(toInt(Global::AVL_FUNC::INSERT), 4);
    balancing(cur);
    if (cur == root) createSnapshot(toInt(Global::AVL_FUNC::INSERT), 4);
    // std::cerr << "Insert recur " << cur->id << " end\n";
    return;
}

bool AVLTree::removeRecur(int x,Node *&cur)
{
    if (!cur) return false;
    cur->ishighlighted = true;
    createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 0);
    bool res = 0;
    if (x < cur->val) {
        res = removeRecur(x, cur->ltCh);
    }
    else if (x > cur->val) {
        res = removeRecur(x,cur->rtCh);
    }
    else {
        createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 1);
        if (!cur->ltCh) {
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 2);
            Node *tmp = cur->rtCh;
            delete cur;
            cur = tmp;
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 3);
        }
        else if (!cur->rtCh) {
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 2);
            Node *tmp = cur->ltCh;
            delete cur;
            cur = tmp;
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 3);
        }
        else {
            int tmpVal = INT_MIN;
            findMax(cur->ltCh,tmpVal);
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 2);
            cur->val = tmpVal;
            createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 3);
        }
        res = 1;
    }

    createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 4);
    if (cur) {
        cur->ishighlighted = false;
        balancing(cur);
    }
    if (cur == root) createSnapshot(toInt(Global::AVL_FUNC::REMOVE), 4);

    return res;
}

void AVLTree::clearRecur(Node *cur) {
    if (!cur) return;
    if (cur->ltCh) clearRecur(cur->ltCh);
    if (cur->rtCh) clearRecur(cur->rtCh);
    delete cur;
}

void AVLTree::insert(int x)
{
    // std::cerr << "AVL insert called\n";
    counter++;
    insertRecur(x,root);
    // std::cerr << "AVL insert end\n";
}
bool AVLTree::find(int x)
{
    Node *cur = root;
    std::stack<Node*> st;
    bool flag = false;
    while (true) {
        if (cur == nullptr) {
            createSnapshot(toInt(Global::AVL_FUNC::FIND), 0);
            return flag;
        }
        cur->ishighlighted = true;
        st.push(cur);
        if (cur->val == x) {
            createSnapshot(toInt(Global::AVL_FUNC::FIND), 1);
            flag = true;
            break;
        }
        if (x < cur->val) {
            cur = cur->ltCh;
            createSnapshot(toInt(Global::AVL_FUNC::FIND), 3);
        }
        else {
            cur = cur->rtCh;
            createSnapshot(toInt(Global::AVL_FUNC::FIND), 2);
        }
    }
    // if (!flag) createSnapshot(toInt(Global::AVL_FUNC::FIND), 0);
    // else createSnapshot(toInt(Global::AVL_FUNC::FIND), 1);

    while (st.size()) {
        st.top()->ishighlighted = false;
        st.pop();
        // createSnapshot();
    }


    return flag;
}
bool AVLTree::remove(int x)
{
    return removeRecur(x,root);
}
bool AVLTree::update(int x,int newVal)
{
    createSnapshot(toInt(Global::AVL_FUNC::UPDATE), 0);
    if (remove(x)) {
        createSnapshot(toInt(Global::AVL_FUNC::UPDATE), 1);
        insert(newVal);
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

// Trie implementation

Trie::~Trie()
{
    this->clear();
}

Trie::Node::Node()
{
    for (int i = 0; i < MAX_SZ; i++) {
        ch[i] = nullptr;
    }
}

void Trie::createSnapshotRecur(Node *cur, Global::TreeStructure &snapshot)
{
    if (!cur) return;
    snapshot.valueMap[cur->id] = std::to_string(cur->cnt);
    std::vector<std::pair<int,char>> chVec;
    for (int i = 0; i < MAX_SZ; i++) {
        if (cur->ch[i]) {
            chVec.emplace_back(cur->ch[i]->id,char('a' + i));
            createSnapshotRecur(cur->ch[i],snapshot);
        }
    }
    snapshot.trieMap[cur->id] = std::make_tuple(cur->ishighlighted,cur->isSpecial,chVec);
}

void Trie::createSnapshot(int func,int line)
{
    snapshot_ptr->push_back(Global::TreeStructure());
    Global::TreeStructure &cur = snapshot_ptr->back();
    cur.codeFunction = func;
    cur.codeLine = line;
    cur.rootId = root ? root->id : -1;
    createSnapshotRecur(root,cur);
}

bool Trie::removeRecur(Node *cur,int id,const std::string &s)
{
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),0);
    if (!cur) return false;
    cur->ishighlighted = true;
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),1);
    if (id == s.size()) {
        cur->cnt--;
        cur->strCnt--;
        if (cur->strCnt == 0) cur->isSpecial = 0;
        createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),2);
        cur->ishighlighted = false;
        createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),3);
        return true;
    }
    int c = s[id] - 'a';
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),4);
    if (!removeRecur(cur->ch[c],id+1,s)) {
        cur->ishighlighted = false;
        createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),4);
        return false;
    }
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),5);
    if (cur->ch[c]->cnt == 0) {
        delete cur->ch[c];
        cur->ch[c] = nullptr;
        createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),6);
        createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),7);
    }
    cur->cnt--;
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),8);
    cur->ishighlighted = false;
    createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),9);
    return true;
}

void Trie::clearRecur(Node *&cur)
{
    if (!cur) return;
    for (int i = 0; i < MAX_SZ; i++) {
        if (cur->ch[i]) clearRecur(cur->ch[i]);
    }
    delete cur;
    cur = nullptr;
}

void Trie::insert(const std::string &s)
{
    std::cerr << "Trie - DS: Inserting: " << s << '\n';
    createSnapshot(toInt(Global::TRIE_FUNC::INSERT),0);
    if (!root) {
        counter++;
        root = new Node();
        root->id = counter;
    }
    Node *cur = root;
    // cur->ishighlighted = true;
    createSnapshot(toInt(Global::TRIE_FUNC::INSERT),1);
    for (int i = 0; i < s.size(); i++) {
        cur->ishighlighted = true;
        createSnapshot(toInt(Global::TRIE_FUNC::INSERT),2);
        int c = s[i] - 'a';
        cur->cnt++;
        createSnapshot(toInt(Global::TRIE_FUNC::INSERT),3);
        createSnapshot(toInt(Global::TRIE_FUNC::INSERT),4);
        if (!cur->ch[c]) {
            counter++;
            cur->ch[c] = new Node();
            cur->ch[c]->id = counter;
            createSnapshot(toInt(Global::TRIE_FUNC::INSERT),5);
        }
        cur->ishighlighted = false;
        cur = cur->ch[c];
        // cur->ishighlighted = true;
        createSnapshot(toInt(Global::TRIE_FUNC::INSERT),6);
    }
    cur->cnt++;
    cur->strCnt++;
    cur->isSpecial = 1;
    createSnapshot(toInt(Global::TRIE_FUNC::INSERT),7);
    cur->ishighlighted = false;
    createSnapshot(toInt(Global::TRIE_FUNC::INSERT),7);
    std::cerr << "Trie - DS: Insert ended\n";
}

bool Trie::find(const std::string &s)
{
    createSnapshot(toInt(Global::TRIE_FUNC::FIND),0);
    if (!root) {
        return false;
    }
    Node *cur = root;
    cur->ishighlighted = true;
    createSnapshot(toInt(Global::TRIE_FUNC::FIND),1);
    for (int i = 0; i < s.size(); i++) {
        createSnapshot(toInt(Global::TRIE_FUNC::FIND),2);
        int c = s[i] - 'a';
        createSnapshot(toInt(Global::TRIE_FUNC::FIND),3);
        if (!cur->ch[c]) {
            cur->ishighlighted = false;
            return false;
        }
        cur->ishighlighted = false;
        cur = cur->ch[c];
        cur->ishighlighted = true;
        createSnapshot(toInt(Global::TRIE_FUNC::FIND),4);
    }
    createSnapshot(toInt(Global::TRIE_FUNC::FIND),5);
    cur->ishighlighted = false;
    return cur->strCnt;
}

bool Trie::remove(const std::string &s)
{
    std::cerr << "Trie - DS: Remove called\n";
    if (removeRecur(root,0,s)) {
        if (root->cnt == 0) {
            delete root;
            root = nullptr;
            createSnapshot(toInt(Global::TRIE_FUNC::REMOVE),-1);
        }
        std::cerr << "Trie - DS: Remove ended true\n";
        return true;
    } 
    std::cerr << "Trie - DS: Remove ended false\n";
    return false;
}

bool Trie::update(const std::string &s, const std::string &newS)
{
    createSnapshot(toInt(Global::TRIE_FUNC::UPDATE),0);
    // bool flag = remove(s);
    // std::cerr << flag << '\n';
    if (remove(s)) {
        createSnapshot(toInt(Global::TRIE_FUNC::UPDATE),1);
        insert(newS);
        return true;
    }
    return false;
}

void Trie::clear()
{
    clearRecur(root);
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