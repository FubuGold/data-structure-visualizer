#ifndef DATA_STRUCTURE_TEST
#define DATA_STRUCTURE_TEST

#include <bits/stdc++.h>

#include "../../include/data-structure/data-structure.h"

using namespace std;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());

int rng(int l,int r)
{
    return uniform_int_distribution<int>(l,r)(rnd);
}

void avlTest()
{
    std::vector<Global::TreeStructure> dummy;
    DataStructure::AVLTree tree;
    tree.linkSnapshot(&dummy);
    vector<int> vec = {15,70,83,71};
    // for (int i=0;i<10;i++) {
    //     vec.push_back(rng(1,100));
    //     std::cerr << "Insert " << i << " element: " << vec.back() << '\n';
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

    std::cerr << "AVL test completeed\n";
}

#endif // DATA_STRUCTURE_TEST