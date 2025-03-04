#include "vga.h"

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;
uint16_t* const VGA_BUFFER = (uint16_t*) 0xb8000;

static vga_color_t terminal_color;
static cursor_t terminal_cursor = {0, 0};

vga_color_t vga_get_color(terminal_colors_t text, terminal_colors_t background) {
    vga_color_t color = {text, background};
    return color;
}

vga_char_t vga_get_char(unsigned char character, vga_color_t color) {
    vga_char_t vga_char = {character, color};
    return vga_char;
}


size_t terminal_get_cursor_index(void) {
    return terminal_cursor.y * VGA_WIDTH + terminal_cursor.x;
}

cursor_t terminal_cursor_index_to_position(size_t index) {
    cursor_t cursor = {index % VGA_WIDTH, index / VGA_WIDTH};
    return cursor;
}

size_t terminal_cursor_position_to_index(cursor_t cursor) {
    return cursor.y * VGA_WIDTH + cursor.x;
}

void terminal_move_cursor(size_t x, size_t y) {
    if (x >= VGA_WIDTH) {
        x = 0;
        ++y;
    }
    if (y >= VGA_HEIGHT) {
        terminal_clear();
        y = 0;
    }
    terminal_cursor.x = x;
    terminal_cursor.y = y;

    size_t index = terminal_cursor_position_to_index(terminal_cursor);
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(index & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((index >> 8) & 0xFF));
}

void terminal_advance_cursor(void) {
    terminal_move_cursor(terminal_cursor.x + 1, terminal_cursor.y);
}

void terminal_newline(void) {
    terminal_move_cursor(0, terminal_cursor.y + 1);
}

void terminal_retrocede_cursor(void) {
    if (terminal_cursor.x == 0) {
        terminal_move_cursor(terminal_cursor.x, VGA_WIDTH);
    } else {
        terminal_move_cursor(terminal_cursor.x - 1, terminal_cursor.y);
    }
}

void terminal_put_char(char c) {
    if (c == '\n') {
        terminal_newline();
        return;
    }
    if (c == '\b') {
        terminal_delete_char();
        return;
    }
    vga_char_t vga_char = vga_get_char(c, terminal_color);
    size_t index = terminal_get_cursor_index();
    VGA_BUFFER[index] = vga_char.character | (vga_char.color.text_color << 8) | (vga_char.color.background_color << 12);
    terminal_advance_cursor();
}

void terminal_delete_char(void) {
    if (terminal_cursor.x == 0) {
        return;
    }
    terminal_retrocede_cursor();
    terminal_put_char(' ');
    terminal_retrocede_cursor();
}

void terminal_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_put_char(data[i]);
    }
}

void terminal_initialize(void) {
    terminal_color = vga_get_color(TERMINAL_COLOR_LIGHT_GREY, TERMINAL_COLOR_BLACK);
    terminal_move_cursor(0, 0);
    terminal_clear();
}

void terminal_clear(void) {
    vga_char_t vga_empty_char = vga_get_char(' ', terminal_color);
    for (size_t index = 0; index < VGA_HEIGHT * VGA_WIDTH; ++index) {
        VGA_BUFFER[index] = vga_empty_char.character | (vga_empty_char.color.text_color << 8) | (vga_empty_char.color.background_color << 12);
    }
}

void terminal_set_color(vga_color_t color) {
    terminal_color = color;
}