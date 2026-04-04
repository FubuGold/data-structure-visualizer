#ifndef DATA_STRUCTURE_TEST
#define DATA_STRUCTURE_TEST

#include <bits/stdc++.h>

#include "../../include/data-structure/data-structure.h"
#include "../../include/data-structure/file-validator.h"

using namespace std;

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

    tree.clear();

    std::cerr << "AVL test completeed\n";

    using Validator::avlValidator;
    if (!avlValidator("./src/test/avlTest1.txt")) {
        std::cerr << "AVL validator test 1 failed\n";
    }
    if (avlValidator("./src/test/avlTest2.txt")) {
        std::cerr << "AVL validator test 2 failed\n";
    }
    if (avlValidator("./src/test/avlTest3.txt")) {
        std::cerr << "AVL validator test 3 failed\n";
    }
    
    std::cerr << "AVL validator test completed\n";
}

#endif // DATA_STRUCTURE_TEST