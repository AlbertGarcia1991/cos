#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "tests/tests.c"

#include "vga.h"
#include "stdutils.h"



void kernel_main(void) {
    terminal_initialize();

    test_functon_vga_basic();

	while (1) {
    }
}