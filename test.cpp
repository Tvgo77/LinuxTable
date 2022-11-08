#include "bpt1.h"
#include <iostream>
#include <vector>


int main() {
    bplus_tree bt("./index", false);
    bt.insert(555UL, 5);
    bt.insert(555UL, 6);
    key_type left = 555UL;
    key_type right = 555UL;
    std::vector<value_t> values;
    bt.search_range(left, right, values, 10);
    return 0;
}