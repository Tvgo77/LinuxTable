#include "bpt1.h"
#include <iostream>

using namespace bpt;

int main() {
    bplus_tree bt("./index", false);
    bt.insert(555UL, 5);
    bt.insert(555UL, 6);
    bpt::key_t left = 555UL;
    bpt::key_t right = 555UL;
    bpt::value_t values[10] = {};
    bt.search_range(left, right, values, 10);
    return 0;
}