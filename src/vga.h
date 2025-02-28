#ifndef VGA_H
#define VGA_H

#include <stdint.h>
#include <stddef.h>

extern void outb(uint16_t port, uint8_t val);

extern uint16_t* const VGA_BUFFER;
extern const size_t VGA_WIDTH;
extern const size_t VGA_HEIGHT;

enum TERMINAL_COLORS {
	TERMINAL_COLOR_BLACK = 0,
	TERMINAL_COLOR_BLUE = 1,
	TERMINAL_COLOR_GREEN = 2,
	TERMINAL_COLOR_CYAN = 3,
	TERMINAL_COLOR_RED = 4,
	TERMINAL_COLOR_MAGENTA = 5,
	TERMINAL_COLOR_BROWN = 6,
	TERMINAL_COLOR_LIGHT_GREY = 7,
	TERMINAL_COLOR_DARK_GREY = 8,
	TERMINAL_COLOR_LIGHT_BLUE = 9,
	TERMINAL_COLOR_LIGHT_GREEN = 10,
	TERMINAL_COLOR_LIGHT_CYAN = 11,
	TERMINAL_COLOR_LIGHT_RED = 12,
	TERMINAL_COLOR_LIGHT_MAGENTA = 13,
	TERMINAL_COLOR_LIGHT_BROWN = 14,
	TERMINAL_COLOR_WHITE = 15,
};
typedef enum TERMINAL_COLORS terminal_colors_t;

struct vga_color {
    terminal_colors_t text_color : 4;
    terminal_colors_t background_color : 4;
};
typedef struct vga_color vga_color_t;


struct vga_char {
    unsigned char character;
    vga_color_t color;
};
typedef struct vga_char vga_char_t;

struct cursor {
    size_t x;
    size_t y;
};
typedef struct cursor cursor_t;

vga_color_t vga_get_color(terminal_colors_t text_color, terminal_colors_t background_color);
vga_char_t vga_get_char(unsigned char character, vga_color_t color);

cursor_t terminal_cursor_index_to_position(size_t index);
size_t terminal_cursor_position_to_index(cursor_t cursor);
size_t terminal_get_cursor_index(void);
void terminal_reset_cursor(void);
void terminal_move_cursor(size_t x, size_t y);
void terminal_advance_cursor(void);
void terminal_newline(void);
void terminal_retrocede_cursor(void);

void terminal_initialize(void);
void terminal_clear(void);
void terminal_set_color(vga_color_t color);

void terminal_put_char(char c);
void terminal_delete_char(void);
void terminal_write(const char* data);

#endif