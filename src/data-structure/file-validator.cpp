#include "../../include/data-structure/file-validator.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>

namespace Validator
{

bool avlValidator(const std::string &filename)
{
    std::ifstream inp(filename);
    int n;
    std::vector<int> vec;
    if (!(inp >> n)) {
        inp.close();
        std::cerr << "Input n failed\n";
        return false;
    }
    // vec.assign(n,0);
    int x;
    for (int i=0;i<n;i++) {
        if (!(inp >> x)) {
            std::cerr << "Input element failed\n";
            return false;
        }
        ws(inp);
    }
    // while (inp) {
    //     ws(inp);
    //     char c = 0; inp >> c;
    //     if (c) {
    //         std::cerr << "Redundant input\n";
    //         return false;
    //     }
    // }
    return true;
}

bool heapValidator(const std::string &filename)
{
    std::ifstream inp(filename);
    int n;
    std::vector<int> vec;
    if (!(inp >> n)) {
        inp.close();
        std::cerr << "Input n failed\n";
        return false;
    }
    // vec.assign(n,0);
    int x;
    for (int i=0;i<n;i++) {
        if (!(inp >> x)) {
            std::cerr << "Input element failed\n";
            return false;
        }
        ws(inp);
    }
    return true;
}

bool sllValidator(const std::string &filename)
{
    std::ifstream inp(filename);
    int n;
    std::vector<int> vec;
    if (!(inp >> n)) {
        inp.close();
        std::cerr << "Input n failed\n";
        return false;
    }
    // vec.assign(n,0);
    int x;
    for (int i=0;i<n;i++) {
        if (!(inp >> x)) {
            std::cerr << "Input element failed\n";
            return false;
        }
        ws(inp);
    }
    // while (inp) {
    //     ws(inp);
    //     char c = 0; inp >> c;
    //     if (c) {
    //         std::cerr << "Redundant input\n";
    //         return false;
    //     }
    // }
    return true;
}

bool trieValidator(const std::string &filename)
{
    std::ifstream inp(filename);
    int n;
    std::vector<int> vec;
    if (!(inp >> n)) {
        inp.close();
        std::cerr << "Input n failed\n";
        return false;
    }
    std::string tmp;
    for (int i=0;i<n;i++) {
        if (!(inp >> tmp)) {
            std::cerr << "Input element failed\n";
            return false;
        }
        for (int j = 0; j < tmp.size(); j++) {
            if (tmp[j] < 'a' || tmp[j] > 'z') return false;
        }
        ws(inp);
    }
    // while (inp) {
    //     ws(inp);
    //     char c = 0; inp >> c;
    //     if (c) {
    //         std::cerr << "Redundant input\n";
    //         return false;
    //     }
    // }
    return true;
}

} // namespace Validator
