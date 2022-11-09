#include "syscall_wrapper.h"
#include "predefined.h"
#include "bpt.h"
#include <random>
#include <sys/stat.h>
#include <vector>
#include <string>
#include <pthread.h> 

using std::string;
using std::vector;

pthread_rwlock_t rwlock;

// Generate a random row
row rand_row_generate();

// Add a row and insert its index to existing b+ tree file
void add_row(row r, const bool * p_attribute, const char * path);

// Search all rows with column value in [left_val, right_val] in which attribute is denoted by column_num 
void search_row(int column_num, column left_val, column right_val, 
                std::vector<row> &result, const bool * p_attribute,
                const char *path);

// Construct table with random value
void table_construct(const char *path);

// Create index file of attributes, attribute_idx indicates the specific attribute
void index_construct(int attribute_idx, bool * p_attribute);

// initialize the global variable attributes
void init_attributes();