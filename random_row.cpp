#include "random_row.h"

row rand_row_generate() {
    std::random_device dev;
    std::uniform_int_distribution<unsigned long> randnum(0, UINT64_MAX);  // Adjust random range here
    std::mt19937 rng(dev());
    row random_row;
    for (int i = 0; i<100; i++) {
        random_row.r[i] = randnum(rng);
    }
    return random_row;
}