#include "syscall_wrapper.h"
#include "random_row.h"
#include "table_type.h"

void add_row(row r) {
    int fd = Open("./table/table", O_RDWR, 0);
    off_t offset_w = Lseek(fd, 0L, SEEK_END);
    ssize_t nbytes_w = Write(fd, &r, 800);
    int rc = Close(fd);
}

int main() {
    row r = rand_row_generate();
    add_row(r);

    // Read last row
    int fd = Open("./table/table", O_RDONLY, 0);
    row output;
    off_t offset = Lseek(fd, -800, SEEK_END);
    ssize_t nbytes_r = Read(fd, &output, 800);
    int rc = Close(fd);
    return 0;
}