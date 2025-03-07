#include <stdint.h>

#include "../vga.h"
#include "../print.h"
#include "../stdutils.h"


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

void test_function_print(void) {
    int8_t a = 10;
    int8_t b = -10;
    uint8_t c = 222;
    uint8_t d = 10;
    char e = 'k';
    str_t f = "STRING";
    print("First regular print.");
    print("Second regular print.\nThis is a new line from the second regular print");
    println("This is the third print, adding a new line since is println");
    println("Here it is the prove of the new line :)");
    printf("Int %d %d, uInt %u, hex %x, char %c", a, b, c, d, e);
    printfln(", string %s", f);

    str_t src = "Original";
    char dst[32];
    strcopy(src, dst);
    printfln("Copy: %s", dst);
}