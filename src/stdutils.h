#ifndef H_STDUTILS
#define H_STDUTILS

#include "typedef.h"

usize_t strlen(const str_t str);

// Numerical <-> String conversions
void int_to_str(int num, const str_t str);
void hex_to_str(int num, const str_t str);

// String utils
void strcopy(const str_t src, const str_t dst);

#endif