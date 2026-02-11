#ifndef DATA_STRUCTURE_CORE
#define DATA_STRUCTURE_CORE

#include <vector>
#include <string>
#include <optional>
#include <utility>

namespace DataStructure
{

template<typename T>
class BaseStructure
{
protected:
    int counter = 0;
public:
    virtual void insert(T x) {};
    virtual bool find(T x) {};
    virtual void remove(T x) {};
    virtual void clear() {};
};

class SinglyLinkedList : public BaseStructure<int>
{
private:
    struct Node
    {
        Node *pNext = nullptr;
        int id,val;
    } *head = nullptr, *tail = nullptr;

public:
    void insert(int x) override;
    bool find(int x) override;
    void remove(int x) override;
    void clear() override;

};

// Max heap
class Heap : public BaseStructure<int>
{
private:
    std::vector<int> vec;
    int findPa(int x);
    int findLt(int x);
    int findRt(int x);
    void heapify(int id);
public:
    void insert(int x) override;
    std::optional<int> getMax();
    void removeMax();
    void clear() override;

};

class AVLTree : public BaseStructure<int>
{
private:
    struct Node
    {
        Node *ltCh = nullptr, *rtCh = nullptr;
        int height = 0;
        int id,val;
        inline int leftHeight();
        inline int rightHeight();
    } *root = nullptr;

    void recalHeight(Node *cur);
    // Return the new current
    Node* leftRotate(Node *cur);
    // Return the new current
    Node* rightRotate(Node *cur);
    // Return the new current. Include recalHeight at the start
    Node* balancing(Node *cur);

    // Get the max and delete node, with reconecting
    Node* findMax(Node *cur, int& retVal);
    Node* insertRecur(int x,Node *cur);
    Node* removeRecur(int x,Node *cur);
    void clearRecur(Node *cur);

public:
    void insert(int x) override;
    bool find(int x) override;
    void remove(int x) override;
    void clear() override;

};

/**
 * @brief Trie for storing string
 * 
 * Note 1: Only from 'a' to 'z'
 * 
 * Node 2: This use copy, not reference or move
 * 
 */
class Trie : BaseStructure<std::string>
{
private:
    static const int CHAR_NUM = 26;
    struct Node
    {
        Node *ch[CHAR_NUM];
        int cnt = 0;
        int exist = 0;
        int id;
    } *root = nullptr;
    
    void clearRecur(Node *cur);
    bool removeRecur(std::string &s, int id, Node *cur);

public:
    void insert(std::string s) override;
    bool find(std::string s) override;
    void remove(std::string s) override;
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
    void setSize(int n);
    void addEdge(int x,int y,int w);
    int findMST();
    int findSP(int st,int ed);
    void clear();
};

};

#endif // DATA_STRUCTURE_CORE