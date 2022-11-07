#include "syscall_wrapper.h"
#include "random_row.h"
#include "table_type.h"
#include "predefined.h"
#include <vector>


void add_row(row r) {
    int fd = Open("./table/table", O_RDWR, 0);
    off_t offset_w = Lseek(fd, 0L, SEEK_END);
    ssize_t nbytes_w = Write(fd, &r, 800);
    int rc = Close(fd);
}

// Search all rows with column value in [left_val, right_val] in which attribute is denoted by column_num 
void search_row(int column_num, column left_val, column right_val, std::vector<row> &result) {
    if (column_num < 0 || column_num > 99) {
        printf("%s", "Invalid column number. The column number must be in 0~99.");
        return;
    }

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

int main() {
    row r = {10UL, 5UL, 5UL};
    add_row(r);

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