#include "table_api.h"
#include <chrono>

struct add_row_para {
    row r;
    const bool *p_attribute;
};

struct search_row_para {
    int column_num;
    column left_val;
    column right_val;
    vector<row> &result;

    search_row_para(vector<row> &v1) : result(v1) {}
};

void *add_row_thr(void *vargp) {
    add_row_para * args = (add_row_para*) vargp;
    add_row(args->r, args->p_attribute);
    //free(vargp);
    return NULL;
}

void *search_row_thr(void *vargp) {
    search_row_para *args = (search_row_para*) vargp;
    search_row(args->column_num, args->left_val, args->right_val, args->result);
    // free(vargp);
    return NULL;
}

void *index_construct_thr(void *vargp) {
    int attribute_idx = *((int*) vargp);
    index_construct(attribute_idx);
    free(vargp);
    return NULL;
}

void test1() {
    table_construct();
}

void test2() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    column left_val = 4651581685022161230UL;
    column right_val = 4651581685022161232UL;
    std::vector<row> result;

    // Accurate normal search 
    auto t1 = high_resolution_clock::now();
    search_row(0, left_val, right_val, result);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms = t2-t1;
    return;
}

void test3() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    index_construct(0);
    index_construct(2);

    column left_val = 4651581685022161231UL;
    column right_val = 4651581685022161231UL;
    std::vector<row> result;

    // Accurate normal search 
    auto t1 = high_resolution_clock::now();
    search_row(0, left_val, right_val, result);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms = t2-t1;
    return;
}

void test4() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;

    // find whether an index file exist
    init_attributes();

    add_row({202222080642UL, 317024848UL, 77UL});
    
    column left_val = 202222080642UL;
    column right_val = 202222080642UL;
    std::vector<row> result;

    // Accurate normal search 
    auto t1 = high_resolution_clock::now();
    search_row(0, left_val, right_val, result);
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms = t2-t1;
    return;
}


void test5() {
    pthread_t tid1, tid2;
    init_attributes();
    
    // add_row({10UL, 5UL, 5UL});

    vector<row> search_result;
    search_row_para search_row_argp(search_result);
    search_row_argp.column_num = 0;
    search_row_argp.left_val = 202222080642UL;
    search_row_argp.right_val = 202222080642UL;

    add_row_para add_row_argp;
    add_row_argp.r = {123UL, 234UL, 345UL};

    int rc1 = pthread_create(&tid2, NULL, add_row_thr, (void*)&add_row_argp);
    int rc2 = pthread_create(&tid1, NULL, search_row_thr, (void*)&search_row_argp);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return;
}

int main() {
    test5();
}