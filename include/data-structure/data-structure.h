#ifndef DATA_STRUCTURE_CORE
#define DATA_STRUCTURE_CORE

#include <vector>
#include <string>
#include <optional>
#include <utility>
#include <map>

#include "../global/global.h"

namespace DataStructure
{

class BaseStructure
{
protected:
    int counter = 0;

    std::vector<Global::TreeStructure> *snapshot_ptr = nullptr;

public:
    virtual ~BaseStructure() = default;

    virtual void insert(int x) {};
    virtual bool find(int x) {};
    virtual bool remove(int x) {};
    virtual bool update(int x,int newVal) {};
    virtual void clear() {};

    virtual void createSnapshot(int func,int line) {};

    // Does not unlink the snapshot list
    void clearSnapshot();
    void linkSnapshot(std::vector<Global::TreeStructure> *snapshot_ptr);
};

class SinglyLinkedList : public BaseStructure
{
private:
    struct Node
    {
        Node *pNext = nullptr;
        int id,val;
        bool ishighlighted;
    } *head = nullptr, *tail = nullptr;

public:
    ~SinglyLinkedList();

    void insert(int x) override;
    bool find(int x) override;
    bool remove(int x) override;
    void clear() override;

};

// Max heap
class Heap : public BaseStructure
{
private:
    struct Node 
    {
        int id,val;
        bool isHighlighted = false;
        Node(){}
        Node(int id,int val) : id(id), val(val) {}
    };

    std::vector<Node> vec;
    inline int findPa(int x);
    inline int findLt(int x);
    inline int findRt(int x);
    void downheap(int id);
    void upheap(int id);

    virtual void createSnapshot(int func,int line) override;

public:
    ~Heap();

    void insert(int x) override;
    int getMax();
    void pop();
    void updateById(int id,int newVal);
    void removeById(int id);
    void clear() override;

};

class AVLTree : public BaseStructure
{
private:
    struct Node
    {
        Node *ltCh = nullptr, *rtCh = nullptr;
        int height = 1;
        int id,val;
        bool ishighlighted = 0;
        inline int leftHeight();
        inline int rightHeight();
    } *root = nullptr;

    void recalHeight(Node *cur);
    void leftRotate(Node *&cur);
    void rightRotate(Node *&cur);
    // Include recalHeight at the start
    void balancing(Node *&cur);

    // Get the max and remove node, with reconecting
    void findMax(Node *&cur, int& retVal);
    void insertRecur(int x,Node *&cur);
    bool removeRecur(int x,Node *&cur);
    void clearRecur(Node *cur);

    void createSnapshotRecur(Node *cur, Global::TreeStructure &structure);

public:
    ~AVLTree();

    void createSnapshot(int func,int line) override;

    void insert(int x) override;
    bool find(int x) override;
    bool remove(int x) override;
    bool update(int x,int newVal) override;
    void clear() override;

};

//======================================================//

// Bidirectional graph
class Graph
{
private:
    std::vector<std::vector<std::pair<int,int>>> adj;
    int n;
public:
    ~Graph();

    void setSize(int n);
    void addEdge(int x,int y,int w);
    int findMST();
    int findSP(int st,int ed);
    void clear();
};

};

#endif // DATA_STRUCTURE_CORE