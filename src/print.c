#include "print.h"
#include "vga.h"
#include "stdutils.h"
#include <stdarg.h>
#include <stdbool.h>

void print(const str_t string) {
    terminal_write(string);
}

void println(const str_t string) {
    print(string);
    terminal_put_char('\n');
}

/**
 * @brief Prints a formatted string to the terminal.
 *
 * Supported format specifiers:
 * - %d: Integer
 * - %u: Unsigned integer
 * - %x: Hexadecimal integer
 * - %c: Character
 * - %s: String
 * - %f: Float (supports precision, e.g., %.2f)
 *
 * @param format The format string.
 * @param ... The values to format and print.
 */
void printf(const str_t format, ...) {
    va_list args;
    va_start(args, format);

    for (str_t p = format; *p != '\0'; p++) {
        if (*p == '%') {
            p++;
            int precision = DEFAULT_FLOAT_PRECISION;

            if (*p == '.') {
                p++;
                precision = 0;
                while (*p >= '0' && *p <= '9') {
                    precision = precision * 10 + (*p - '0');
                    p++;
                }
            }

            switch (*p) {
                case 'd': {
                    int value = va_arg(args, int);
                    char buffer[32];
                    int_to_str(value, buffer);
                    terminal_write(buffer);
                    break;
                }
                case 'u': {
                    unsigned int value = va_arg(args, unsigned int);
                    char buffer[32];
                    int_to_str(value, buffer);
                    terminal_write(buffer);
                    break;
                }
                case 'x': {
                    unsigned int value = va_arg(args, unsigned int);
                    char buffer[32];
                    hex_to_str(value, buffer);
                    terminal_write(buffer);
                    break;
                }
                case 'c': {
                    char value = (char)va_arg(args, int);
                    terminal_put_char(value);
                    break;
                }
                case 's': {
                    str_t value = va_arg(args, str_t);
                    terminal_write(value);
                    break;
                }
                // TODO: Float to string
                // case 'f': {
                //     double value = va_arg(args, double);
                //     char buffer[64];
                //     float_to_string(value, buffer, precision);
                //     terminal_write(buffer);
                //     break;
                // }
                default:
                    terminal_put_char('%');
                    terminal_put_char(*p);
                    break;
            }
        } else {
            terminal_put_char(*p);
        }
    }
    va_end(args);
}

void printfln(const str_t format, ...) {
    printf(format);
    terminal_put_char('\n');
}