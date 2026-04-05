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
    vec.assign(n,0);
    for (int i=0;i<n;i++) {
        if (!(inp >> vec[i])) {
            std::cerr << "Input element failed\n";
            return false;
        }
        ws(inp);
    }
    while (inp) {
        ws(inp);
        char c = 0; inp >> c;
        if (c) {
            std::cerr << "Redundant input\n";
            return false;
        }
    }
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
    vec.assign(n,0);
    for (int i=0;i<n;i++) {
        if (!(inp >> vec[i])) {
            std::cerr << "Input element failed\n";
            return false;
        }
        ws(inp);
    }
    while (inp) {
        ws(inp);
        char c = 0; inp >> c;
        if (c) {
            std::cerr << "Redundant input\n";
            return false;
        }
    }
    return true;
}

} // namespace Validator
