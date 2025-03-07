#ifndef H_TYPEDEF
#define H_TYPEDEF

#define DEFAULT_FLOAT_PRECISION     6

#define true    1
#define false   0

#ifdef __USIZE_TYPE__
#undef __USIZE_TYPE__
#endif
#define __USIZE_TYPE__ long unsigned int
typedef __USIZE_TYPE__ usize_t;

#ifdef __SIZE_TYPE__
#undef __SIZE_TYPE__
#endif
#define __SIZE_TYPE__ long int
typedef __SIZE_TYPE__ ssize_t;

#ifdef __FLOAT_TYPE__
#undef __FLOAT_TYPE__
#endif
#define __FLOAT_TYPE__ float
typedef __FLOAT_TYPE__ float32_t;

#ifdef __DOUBLE_TYPE__
#undef __DOUBLE_TYPE__
#endif
#define __DOUBLE_TYPE__ double
typedef __DOUBLE_TYPE__ float64_t;

#ifdef __UCHAR_TYPE__
#undef __UCHAR_TYPE__
#endif
#define __UCHAR_TYPE__ unsigned char
typedef __UCHAR_TYPE__ uchar_t;

typedef char* str_t;

#include <float.h>



#endif