#include "bpt1.h"
#include <iostream>
#include <vector>
#include <algorithm>


int main() {
    int a[5] = {1, 3, 5, 7, 9};
    void *p = std::lower_bound(std::begin(a), std::end(a), 0);
    return 0;
}