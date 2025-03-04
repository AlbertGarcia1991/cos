#include "stdutils.h"

usize_t strlen(const char* str) 
{
	usize_t len = 0;
	while (str[len])
		len++;
	return len;
}