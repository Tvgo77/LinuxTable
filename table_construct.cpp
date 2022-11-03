#include "syscall_wrapper.h"
#include "random_row.h"
#include "table_type.h"

#define N 100  // initial row numbers

// Write 100 random rows to file
int main() {  
    row random_rows[N];
    for (int i = 0; i < N; i++) 
        random_rows[i] = rand_row_generate();
    

    int fd = Open("./table/table", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    off_t offset_w = Lseek(fd, 0L, SEEK_END);
    ssize_t nbytes_w = Write(fd, &random_rows, 800*N);


    // Read first row
    column output[100];
    column *p1 = (column *)output;
    row *p2 = (row *)output;
    off_t offset_r = Lseek(fd, 0, SEEK_SET);
    ssize_t nbytes_r = Read(fd, &output, 800);
    Close(fd);
    return 0;
}
