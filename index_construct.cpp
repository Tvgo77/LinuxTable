#include "bpt1.h"
#include "syscall_wrapper.h"
#include "random_row.h"
#include "table_type.h"
#include <string>

using namespace bpt;
using std::string;

int main() {
    int attribute_idx = 0;
    string index_path = string("./table/index") + std::to_string(attribute_idx);
    bplus_tree bt(index_path.c_str(), true);
    int fd = Open("./table/table", O_RDONLY, 0);
    row buf[N_ROWS];
    ssize_t nbytes_r;
    ssize_t total_bytes_r = 0;
    while((nbytes_r = Read(fd, buf, 800*N_ROWS)) > 0) {
        int valid_row = nbytes_r/800;
        for (int i = 0; i < valid_row; i++) {
            column *p = (column*) &buf[i];
            column key_column = *(p+attribute_idx);  // First column as index
            off_t row_offset = total_bytes_r + 800*i;
            bt.insert(key_column, row_offset);
        }
        total_bytes_r += nbytes_r;
    }



    bpt::key_t left = 764139532788682460UL;
    bpt::key_t right = 764139532788682470UL;
    bpt::value_t values[200] = {};
    bt.search_range(left, right, values, 200);
    return 0;
}