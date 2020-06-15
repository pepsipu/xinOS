#include <stdint.h>

typedef struct
{
    uint16_t x;
    uint16_t y;
} cursor_t;

cursor_t cursor = {
    .x = 0,
    .y = 0,
};

void write_char(uint8_t c, uint8_t fg, uint8_t bg, uint16_t x, uint16_t y)
{
    *((volatile uint16_t *)0xb8000 + (y * 80 + x)) = c | (((bg << 4) | (fg & 0x0f)) << 8);
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
    if (c == '\n' || cursor.x + 1 > 80)
    {
        cursor.x = 0;
        ++cursor.y;
        if (c == '\n')
            return;
    }
    write_char(c, fg, bg, cursor.x++, cursor.y);
}