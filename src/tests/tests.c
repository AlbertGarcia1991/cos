#include "../vga.h"


void test_function_vga_basic(void) {
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
}