#ifndef DATA_STRUCTURE_CORE
#define DATA_STRUCTURE_CORE

#include <vector>
#include <string>

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
        Node *pNext;
        int id,val;
    } *head = nullptr;

public:
    void insert(int x) override;
    bool find(int x) override;
    void remove(int x) override;
    void clear() override;

};

class Heap : public BaseStructure<int>
{
private:
    std::vector<int> vec;
    void heapify();

public:
    void insert(int x) override;
    bool find(int x) override;
    void remove(int x) override;
    void clear() override;

};

class AVLTree : public BaseStructure<int>
{
private:
    struct Node
    {
        Node *lt_ch, *rt_ch;
        int id,val;
    } *root = nullptr;

    Node* rightRotate(Node *cur);
    Node* leftRotate(Node *cur);

public:
    void insert(int x) override;
    bool find(int x) override;
    void remove(int x) override;
    void clear() override;

};

class Trie : BaseStructure<std::string>
{
private:
    struct Node
    {
        Node *ch[26];
        bool exist;
        int id;
    } *root = nullptr;
    
public:
    void insert(std::string x) override;
    bool find(std::string x) override;
    void remove(std::string x) override;
    void clear() override;

};

//======================================================//

class Graph
{
private:
    std::vector<std::vector<int>> adj;
public:
    void addEdge(int x,int y);
    void findMST();
    void findSP(int x,int y);
};

};

#endif // DATA_STRUCTURE_CORE