#include "tests/tests.c"

#include "vga.h"
#include "stdutils.h"



void kernel_main(void) {
    terminal_initialize();

    test_function_vga_basic();

	while (1) {
    }
}