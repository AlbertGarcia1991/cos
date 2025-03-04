#ifndef H_PRINT
#define H_PRINT

#include "typedef.h"

struct Stream {
    usize_t buf_len;
    usize_t buf_i;
    str_t buf;
};

typedef struct Stream stream_t;

// To stdout
void printf(const str_t format, ...);

// To given array
ssize_t sprintf(str_t buffer, const str_t format, ...);
ssize_t snprintf(str_t buffer, const str_t format, usize_t n, ...);

// To returned stream

stream_t test_function();

#endif