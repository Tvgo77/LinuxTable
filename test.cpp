#include "table_api.h"

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
    free(vargp);
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

int main() {
    pthread_t tid1, tid2;
    init_attributes();

    vector<row> search_result;
    search_row_para search_row_argp(search_result);
    search_row_argp.column_num = 0;
    search_row_argp.left_val = 6768528264644406733UL;
    search_row_argp.right_val = 6768528264644406733UL;

    int rc1 = pthread_create(&tid1, NULL, search_row_thr, (void*)&search_row_argp);
    int rc2 = pthread_join(tid1, NULL);
    return 0;
}