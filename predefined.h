#pragma once

#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>

/* num of rows processed per r/w IO*/
#define N_ROWS 100

/* predefined B+ info */
#define BP_ORDER 20

/* key/value type */
typedef off_t value_t;
typedef unsigned long key_type;

static bool attributes[100] = {};

/* type of colum and row*/
typedef unsigned long column;
typedef struct row {column r[100];} row;

#define OPERATOR_KEYCMP(type) \
    bool operator< (const key_type &l, const type &r) {\
        return l < r.key;\
    }\
    bool operator< (const type &l, const key_type &r) {\
        return l.key < r;\
    }\
    bool operator== (const key_type &l, const type &r) {\
        return l == r.key;\
    }\
    bool operator== (const type &l, const key_type &r) {\
        return l.key == r;\
    }


#endif /* end of PREDEFINED_H */
