#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tests/tests.c"

#include "vga.h"
// #include "stdutils.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void entry_point(void) {
    terminal_initialize();

    test_functon_vga_basic();

	while (1) {
    }
}