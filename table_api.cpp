#include "table_api.h"

/*  Shared value or File:
        static bool attributes[100]
        ./table/table
        ./table/index

    API which read the shared resource:
        search_row()  // read table and index
        index_construct()  // read index
        add_row()  // read index

    API which write to the shared resource:
        add_row()  // write to table and index
        index_construct()  // write to attributes and index

    Strategy:
        lock write and read when writes to shared resource
        lock write when read shared resource
*/

pthread_rwlock_t rwlock;

// Generate a random row
row rand_row_generate() {
    std::random_device dev;
    std::uniform_int_distribution<unsigned long> randnum(1, UINT64_MAX);  // Adjust random range here
    std::mt19937 rng(dev());
    row random_row;
    for (int i = 0; i<100; i++) {
        random_row.r[i] = randnum(rng);
    }
    return random_row;
}

// Add a row and insert its index to existing b+ tree file
void add_row(row r, const bool * p_attribute, const char * path) {
    // Write to table file
    pthread_rwlock_wrlock(&rwlock);
    int fd = Open(path, O_RDWR, 0);
    off_t offset_w = Lseek(fd, 0L, SEEK_END);
    ssize_t nbytes_w = Write(fd, &r, 800);
    int rc = Close(fd);
    pthread_rwlock_unlock(&rwlock);

    for (int i = 0; i < 100; i++) {
        if (p_attribute[i]) {
            string index_path = string("./table/index") + std::to_string(i);

            // Write to index file
            pthread_rwlock_wrlock(&rwlock);
            bplus_tree bt(index_path.c_str());
            column * p_col = (column*) &r;
            bt.insert(p_col[i], offset_w);
            pthread_rwlock_unlock(&rwlock);
        }
    }
}

// Search all rows with column value in [left_val, right_val] in which attribute is denoted by column_num 
void search_row(int column_num, column left_val, column right_val, 
                std::vector<row> &result, size_t max, const bool * p_attribute,
                const char *path) {
    if (column_num < 0 || column_num > 99) {
        printf("%s", "Invalid column number. The column number must be in 0~99.");
        return;
    }

    if (!p_attribute[column_num]) {

        // Read table file
        pthread_rwlock_rdlock(&rwlock);
        int fd = Open(path, O_RDONLY, 0);
        row buf[N_ROWS];
        ssize_t nbytes_r;
        // Travel through whole table to get the result
        while((nbytes_r = Read(fd, buf, 800*N_ROWS)) > 0) {
            int valid_row = nbytes_r/800;
            for (int i = 0; i < valid_row; i++) {
                column *p = (column*) &buf[i];
                column value = *(p+column_num);
                if (left_val <= value && value <= right_val)
                    result.push_back(buf[i]);
            }
        }
        Close(fd);
        pthread_rwlock_unlock(&rwlock);
    }

    else {
        string index_path = string("./table/index") + std::to_string(column_num);
        bplus_tree bt(index_path.c_str());
        vector<value_t> values;

        // Read index file and table file
        pthread_rwlock_rdlock(&rwlock);
        bt.search_range(left_val, right_val, values, max);
        int fd = Open(path, O_RDONLY, 0);
        for (int i = 0; i < values.size(); i++) {
            row output;
            off_t offset = Lseek(fd, values[0], SEEK_SET);
            ssize_t nbytes_r = Read(fd, &output, 800);
            result.push_back(output);
        }
        int rc = Close(fd);
        pthread_rwlock_unlock(&rwlock);
    }
    return;
}


// Construct table with random value
void table_construct(const char *path) {
    row random_rows[N_ROWS];
    size_t total_rows = 0;
    
    // Write N_ROWS rows to table file every while loop
    int fd = Open(path, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    while (total_rows < 100000) {
        for (int i = 0; i < N_ROWS; i++) 
            random_rows[i] = rand_row_generate();
        off_t offset_w = Lseek(fd, 0L, SEEK_END);
        ssize_t nbytes_w = Write(fd, &random_rows, 800*N_ROWS);
        total_rows += N_ROWS;
    }
    Close(fd);
    return;
}

// Create index file of attributes, attribute_idx indicates the specific attribute
void index_construct(int attribute_idx, bool * p_attribute) {
    if (attribute_idx < 0 || attribute_idx > 99) {
        printf("%s\n", "Attribute index must be in [0, 99]");
        return;
    }

    // Write to attributes
    pthread_rwlock_wrlock(&rwlock);
    p_attribute[attribute_idx] = 1;
    pthread_rwlock_unlock(&rwlock);

    string index_path = string("./table/index") + std::to_string(attribute_idx);

    // Create (Write to) index file
    pthread_rwlock_wrlock(&rwlock);
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
            bt.insert(key_column, row_offset);  // Write to index file
        }
        total_bytes_r += nbytes_r;
    }
    Close(fd);
    pthread_rwlock_unlock(&rwlock);
    return;
}

// initialize the global variable attributes
void init_attributes(bool * p_attributes) {
    for (int i = 0; i<100; i++) {
        string index_path = string("./table/index") + std::to_string(i);
        struct stat buffer;
        int rc;
        if ((rc = stat(index_path.c_str(), &buffer)) == 0) 
            p_attributes[i] = true;
    }
}
