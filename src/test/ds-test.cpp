#ifndef DATA_STRUCTURE_TEST
#define DATA_STRUCTURE_TEST

#include <bits/stdc++.h>

#include "../../include/data-structure/data-structure.h"
#include "../../include/data-structure/file-validator.h"

using namespace std;

namespace Test
{

mt19937 rnd(998244353);
int rng(int l,int r)
{
    return uniform_int_distribution<int>(l,r)(rnd);
}

}

void avlTest()
{
    vector<Global::TreeStructure> dummy;
    DataStructure::AVLTree tree;
    tree.linkSnapshot(&dummy);
    vector<int> vec = {15,70,83,71,32,1,34};
    // for (int i=0;i<10;i++) {
    //     vec.push_back(rng(1,100));
    //     cerr << "Insert " << i << " element: " << vec.back() << '\n';
    //     tree.insert(vec.back());
    // }

    for (int i=0;i<vec.size();i++) {
        tree.insert(vec[i]);
    }

    for (int i=0;i<vec.size();i++) {
        assert(tree.find(vec[i]));
    }

    for (int i=101;i<110;i++) {
        assert(!tree.find(i));
        assert(!tree.find(i - 111));
    }

    for (int i=0;i<vec.size();i++) {
        tree.remove(vec[i]);
    }

    tree.clear();

    cerr << "AVL test completeed\n";

    using Validator::avlValidator;
    if (!avlValidator("./data/avlTest1.txt")) {
        cerr << "AVL validator test 1 failed\n";
    }
    if (avlValidator("./data/avlTest2.txt")) {
        cerr << "AVL validator test 2 failed\n";
    }
    if (avlValidator("./data/avlTest3.txt")) {
        cerr << "AVL validator test 3 failed\n";
    }
    
    cerr << "AVL validator test completed\n";
}

void heapTest()
{
    vector<Global::TreeStructure> dummy;
    DataStructure::Heap tree;
    tree.linkSnapshot(&dummy);

    priority_queue<int> pq;
    int n = Test::rng(5,20);
    for (int i=0;i<n;i++) {
        int val = Test::rng(0,99);
        pq.push(val);
        tree.insert(val);
        // cerr << val << '\n';
        assert(pq.top() == tree.getMax());
    }

    while (pq.size() > 1) {
        pq.pop();
        tree.pop();
        assert(pq.top() == tree.getMax());
        // cerr << tree.getMax() << ' ' << pq.top() << ' ' << pq.size() << '\n';
    }
    pq.pop();
    tree.pop();
    

    cerr << "Heap test completed\n";

    using Validator::heapValidator;
    if (!heapValidator("./data/avlTest1.txt")) {
        cerr << "AVL validator test 1 failed\n";
    }
    if (heapValidator("./data/avlTest2.txt")) {
        cerr << "AVL validator test 2 failed\n";
    }
    if (heapValidator("./data/avlTest3.txt")) {
        cerr << "AVL validator test 3 failed\n";
    }
    
    cerr << "Heap validator test completed\n";
}

void sllTest()
{
    vector<Global::TreeStructure> dummy;
    DataStructure::SinglyLinkedList ls;
    ls.linkSnapshot(&dummy);

    vector<int> vec;

    int n = Test::rng(5,20);
    for (int i=0;i<n;i++) {
        int val = Test::rng(0,99);
        ls.insert(val);
        vec.push_back(val);
        // cerr << val << '\n';
        // assert(pq.top() == tree.getMax());
    }

    for (int i=0;i<n;i++) {
        ls.find(vec[i]);
    }

    ls.clear();

    for (int i=n-1;i>=0;i--) {
        ls.remove(vec[i]);
    }

    cerr << "SLL test completed\n";

    using Validator::sllValidator;
    if (!sllValidator("./data/avlTest1.txt")) {
        cerr << "SLL validator test 1 failed\n";
    }
    if (sllValidator("./data/avlTest2.txt")) {
        cerr << "SLL validator test 2 failed\n";
    }
    if (sllValidator("./data/avlTest3.txt")) {
        cerr << "SLL validator test 3 failed\n";
    }
    
    cerr << "SLL validator test completed\n";
}

#endif // DATA_STRUCTURE_TEST