#pragma once

#ifndef PREDEFINED_H
#define PREDEFINED_H

#include <string.h>

/* num of rows processed per read/write IO*/
#define N_ROWS 100

/* predefined B+ info */
#define BP_ORDER 30

/* key/value type */
typedef off_t value_t;  // Offset of a row in table file
typedef unsigned long key_type;  // 8 bytes key value

/* a static array indicates whether an index file of attributes exist*/
static bool attributes[100] = {};

/* type of colum and row*/
typedef unsigned long column;
typedef struct row {column r[100];} row;

/* Define compare function of struct index_t and record_t*/
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
