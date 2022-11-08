#include "syscall_wrapper.h"
#include "random_row.h"
#include "table_type.h"
#include "predefined.h"
#include "bpt1.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

// Add a row and insert index to existed b+ tree
void add_row(row r, const bool * p_attribute = attributes) {
    int fd = Open("./table/table", O_RDWR, 0);
    off_t offset_w = Lseek(fd, 0L, SEEK_END);
    ssize_t nbytes_w = Write(fd, &r, 800);
    int rc = Close(fd);

    for (int i = 0; i < 100; i++) {
        if (p_attribute[i]) {
            string index_path = string("./table/index") + std::to_string(i);
            bplus_tree bt(index_path.c_str());
            column * p_col = (column*) &r;
            bt.insert(p_col[i], offset_w);
        }
    }
}

// Search all rows with column value in [left_val, right_val] in which attribute is denoted by column_num 
void search_row(int column_num, column left_val, column right_val, 
                std::vector<row> &result, const bool * p_attribute = attributes) {
    if (column_num < 0 || column_num > 99) {
        printf("%s", "Invalid column number. The column number must be in 0~99.");
        return;
    }

    if (!p_attribute[column_num]) {
        int fd = Open("./table/table", O_RDONLY, 0);
        row buf[N_ROWS];
        ssize_t nbytes_r;
        while((nbytes_r = Read(fd, buf, 800*N_ROWS)) > 0) {
            int valid_row = nbytes_r/800;
            for (int i = 0; i < valid_row; i++) {
                column *p = (column*) &buf[i];
                column value = *(p+column_num);
                if (left_val <= value && value <= right_val)
                    result.push_back(buf[i]);
            }
        }
    }

    else {
        string index_path = string("./table/index") + std::to_string(column_num);
        bplus_tree bt(index_path.c_str());
        vector<value_t> values;
        bt.search_range(left_val, right_val, values, 10UL);

        int fd = Open("./table/table", O_RDONLY, 0);
        row output;
        off_t offset = Lseek(fd, values[0], SEEK_SET);
        ssize_t nbytes_r = Read(fd, &output, 800);
        int rc = Close(fd);
    }
    return;
}

void table_construct(const char *path = "./table/table") {
    row random_rows[N_ROWS];
    size_t total_rows = 0;
    
    int fd = Open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    while (total_rows < 10000) {
        for (int i = 0; i < N_ROWS; i++) 
            random_rows[i] = rand_row_generate();
        off_t offset_w = Lseek(fd, 0L, SEEK_END);
        ssize_t nbytes_w = Write(fd, &random_rows, 800*N_ROWS);
        total_rows += N_ROWS;
    }

    Close(fd);
    return;
}

void index_construct(int attribute_idx, bool * p_attribute = attributes) {
    if (attribute_idx < 0 || attribute_idx > 99) {
        printf("%s\n", "Attribute index must be in [0, 99]");
        return;
    }

    p_attribute[attribute_idx] = 1;

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
    return;
}

int main() {
    table_construct();
    row r = {10UL, 5UL, 5UL};
    add_row(r);

    index_construct(0);

    // Read last row
    int fd = Open("./table/table", O_RDONLY, 0);
    row output;
    off_t offset = Lseek(fd, -800, SEEK_END);
    ssize_t nbytes_r = Read(fd, &output, 800);
    int rc = Close(fd);

    std::vector<row> result;
    search_row(0, 9UL, 11UL, result);
    return 0;
}