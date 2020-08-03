#include <stdint.h>
#include <lib/mem.h>

typedef struct
{
    uint16_t x;
    uint16_t y;
} cursor_t;

cursor_t cursor = {
    .x = 0,
    .y = 0,
};
uint8_t rows;
uint16_t columns;

void init_cmd() {
    columns = *((uint16_t *) 0x44a);
    // cant wait for some chad to pull up with 256 line terminal
    rows = *((uint8_t *) 0x484);
}

void write_char(uint8_t c, uint8_t fg, uint8_t bg, uint16_t x, uint16_t y)
{
    *((volatile uint16_t *)0xb8000 + (y * columns + x)) = c | (((bg << 4) | (fg & 0x0f)) << 8);
}

void scroll() {
    for (int current_row = 1; current_row < rows; ++current_row) {
        volatile uint16_t *before_row = 0xb8000 + (current_row - 1) * columns;
        memcpy(before_row, before_row + columns, columns * 2);
    }
}

void write_str(char *s, uint8_t fg, uint8_t bg)
{
    for (char *ptr = s; *ptr != '\x00'; ++ptr)
    {
        putc(*ptr, fg, bg);
    }
}

void putc(char c, uint8_t fg, uint8_t bg)
{
    if (c == '\n' || cursor.x + 1 > columns)
    {
        cursor.x = 0;
        if (cursor.y + 1 > rows) {
            scroll();
        } else {
            ++cursor.y;
        };
        if (c == '\n')
            return;
    }
    write_char(c, fg, bg, cursor.x++, cursor.y);
}