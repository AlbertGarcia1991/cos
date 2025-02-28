#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    // terminal_write("Hello, Worldfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    // terminal_delete_char();
    // terminal_delete_char();
    // terminal_write("hello");
    // terminal_newline();
    // terminal_write("hello");

    terminal_write("hello");
    terminal_move_cursor(0, 2);
    terminal_put_char('a');
    terminal_put_char('b');
    terminal_put_char('c');
    terminal_put_char('d');
    terminal_delete_char();
    terminal_newline();
    terminal_write("hello");
    terminal_newline();
    terminal_newline();
    terminal_delete_char();
    terminal_delete_char();


    for (size_t i = 0; i < 100; i++) {
    }
	while (1) {
    }
}